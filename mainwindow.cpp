#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QtCore>
#include <math.h>
#include "windows.h"
#include "ising_model.h"

//constructor - initialization
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), //call constructor of base class
    p(0.55), l(new square_lattice(64, 64)), ui(new Ui::MainWindow) //initialize "ui" field by pointer to newly created object
{
    l->fill_random();
    ui->setupUi(this);

    //create dynamic widget
    paintWidget = new PaintWidget(this);
    //setup size, position and other attributes using layout
    //try to put similar widget on form and see build-drawing-Desktop_Qt_5_3_MinGW_32bit-Debug/ui_mainwindow.h
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(paintWidget->sizePolicy().hasHeightForWidth());
    paintWidget->setSizePolicy(sizePolicy);
    ui->horizontalLayout->addWidget(paintWidget);

    //setup status bar
    lb1 = new QLabel("step: ", this);
    statusBar()->addWidget(lb1);
    lb2 = new QLabel("0", this);
    lb2->setMinimumWidth(50);
    statusBar()->addWidget(lb2);
    /*lb3 = new QLabel("Y: ", this);
    statusBar()->addWidget(lb3);
    lb4 = new QLabel(this);
    lb4->setMinimumWidth(50);
    statusBar()->addWidget(lb4);*/

    //setup connection between source of event (that generates/emits "signal") and destination (handler, "slot", that process event)
    connect(paintWidget, SIGNAL(paint_resized(QSize, QSize)), this, SLOT(paint_resized(QSize, QSize)));

    // Создание потока
    QThread* thread = new QThread;
    Worker* worker = new Worker(p, l);

    // Передаем права владения "рабочим" классом, классу QThread.
    worker->moveToThread(thread);

    // Соединяем сигнал started потока, со слотом process "рабочего" класса, т.е. начинается выполнение нужной работы.
    connect(thread, SIGNAL(started()), worker, SLOT(process()));

    // Отображаем в главном потоке Gui значения из вторичного потока
    connect(worker, SIGNAL(sendNumber(int)), this, SLOT(Recieve_data(int)));

    // Оповещаем поток, что нужно остановиться
    connect(this, SIGNAL(SendDeleteThread()), worker, SLOT(RecieveDeleteThread()), Qt::DirectConnection);
    connect(this, SIGNAL(SendRun()), worker, SLOT(RecieveRun()), Qt::DirectConnection);
    connect(this, SIGNAL(SendPause()), worker, SLOT(RecievePause()), Qt::DirectConnection);
    connect(this, SIGNAL(close()), this, SLOT(Time_to_close()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(button_2_clicked()));
    //connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(Time_to_close()));

    // Передача потоку модели
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(button_clicked()));
    //connect(this, SIGNAL(Send_model(parameters, lattice*)), worker, SLOT(Recieve_model(parameters, lattice*)), Qt::DirectConnection);

    // ВЫЗЫВАЕТ УТЕЧКУ ПАМЯТИ
    //connect(worker, SIGNAL(finished()), thread, SLOT(quit()));

    // По завершению выходим из потока, и удаляем рабочий класс
    connect(worker, SIGNAL(destroyed(QObject*)), thread, SLOT(quit()));  // ТАК ПРАВИЛЬНО
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    //connect(this, SLOT(close()), worker, SLOT(RecieveDeleteThread()));

    // Удаляем поток, после выполнения операции
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    //can_draw = false;

    thread->start();
}

//destructor - free resources
MainWindow::~MainWindow()
{
    Stop = true;
    //qDebug() << Stop;
    SendDeleteThread();
    qDebug() << "destruction MainWindow";
    delete lb1;
    delete lb2;
    delete paintWidget;
    delete ui;
    delete l;
}

void MainWindow::Recieve_data(int number)
{
    //ui->lineEdit->setText(QString::number(number));
    //qDebug() << "recieved " << number;
    lb2->setText(QString::number(number));
    draw_picture();
    repaint();
}

//called when button is clicked and form and widget is resized, including creation of form
void MainWindow::draw_picture()
{
    if (l != nullptr)
    {
        QPainter painter(paintWidget->image);
        int w = paintWidget->width(), h = paintWidget->height();
        int size = sqrt(l->getN());
        //painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.setPen(Qt::black);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if ((l->getL())[i + size * j] < 0)
                    painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
                else
                    painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
                painter.drawRect(i*w/size, j*h/size, w/size, h/size);
            }
        }
    }
    else
    {
        paintWidget->image->fill(0);
    }
}

//Run
void MainWindow::button_clicked()
{
    /*can_draw = !can_draw;
    paintWidget->image->fill(0);
    draw_picture();
    repaint();*/
    //Send_model(p, l);
    qDebug() << "run send";
    SendRun();
}

//Pause
void MainWindow::button_2_clicked()
{
    qDebug() << "pause send";
    SendPause();
    //SendDeleteThread();
}

//Exit
void MainWindow::Time_to_close()
{
    qDebug() << "exiting";
    SendDeleteThread();
    Sleep(100); //let thread distruct
    //this->close();
}

void MainWindow::paint_resized(QSize old_size, QSize new_size)
{
    draw_picture();
}

/*void QWidget::closeEvent(QCloseEvent *event)
{
    //Здесь код
    event->accept();
}*/
