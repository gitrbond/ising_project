#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QtCore>
#include <math.h>
#include <QThread>
#include "ising_model.h"

//constructor - initialization
MainWindow::MainWindow(int l_size, QWidget *parent) :
	QMainWindow(parent), alg(0), //call constructor of base class
	p(0.5), l_size(l_size), l(new square_lattice(l_size)), ui(new Ui::MainWindow) //initialize "ui" field by pointer to newly created object
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
    lb3 = new QLabel("Heat bath algorithm", this);
    statusBar()->addWidget(lb3);

    // Сигнал, что картинку нужно перерисовать при изменении размера окна
    connect(paintWidget, SIGNAL(paint_resized(QSize, QSize)), this, SLOT(paint_resized(QSize, QSize)));

    // Создание потока
    QThread* thread = new QThread;
    Worker* worker = new Worker(&alg, &Thread_status, p, l);

    // Передаем права владения "рабочим" классом, классу QThread
    worker->moveToThread(thread);

    // Соединяем сигнал started потока, со слотом process "рабочего" класса, т.е. начинается выполнение нужной работы
    connect(thread, SIGNAL(started()), worker, SLOT(process()));

    // Общение главного потока Gui со вторичным потоком
	connect(worker, SIGNAL(SendStep(int)), this, SLOT(RecieveStep(int)));

    // Сигналы, отправляемые потоку
	// Обработка сигналов кнопок и полей ввода
	connect(this, SIGNAL(SendDeleteThread()), worker, SLOT(RecieveDeleteThread()), Qt::DirectConnection);
	connect(ui->RunButton, SIGNAL(clicked()), worker, SLOT(RecieveRun()), Qt::DirectConnection);
	connect(ui->StopButton, SIGNAL(clicked()), worker, SLOT(RecievePause()), Qt::DirectConnection);
	connect(ui->ChangeAlgoButton, SIGNAL(clicked()), worker, SLOT(RecieveChangeAlgo()), Qt::DirectConnection);
	connect(ui->BetaSpinBox, SIGNAL(valueChanged(double)), worker, SLOT(RecieveNewBeta(double)), Qt::DirectConnection);
	connect(ui->ChangeAlgoButton, SIGNAL(clicked()), this, SLOT(Change_algo_label()));

	// По завершении выходим из потока, и удаляем рабочий класс
    connect(worker, SIGNAL(destroyed(QObject*)), thread, SLOT(quit()));  // ТАК ПРАВИЛЬНО
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    // Удаляем поток, после выполнения расчетов
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    Thread_status = true;
    thread->start();
}

//destructor - free resources
MainWindow::~MainWindow()
{
    SendDeleteThread();
	//qDebug() << "destruction MainWindow";
    delete lb1;
    delete lb2;
    delete paintWidget;
    delete ui;
    delete l;
    int i;
    for (i = 0; i < 50 && Thread_status; i++)
		QThread::msleep(100);
	/*
	if (i == 50)
        qDebug() << "exiting without thread :(";
    else
		qDebug() << "exiting succesfully!";
	*/
}

void MainWindow::RecieveStep(int number)
{
    lb2->setText(QString::number(number));
	draw_picture();
	repaint();
}

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

void MainWindow::Change_algo_label()
{
	if (alg == 0)
        lb3->setText("Heat bath algorithm");
	if (alg == 1)
		lb3->setText("Clusters algorithm");
}

//called when form and widget is resized, repaints widget
void MainWindow::paint_resized(QSize old_size, QSize new_size)
{
	if (old_size != new_size)
		draw_picture();
}
