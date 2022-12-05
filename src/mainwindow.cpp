#include "hdr/mainwindow.h"
#include "hdr/ising_model.h"
#include "hdr/worker.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QWidget>
#include <QDebug>
#include <QtCore>
#include <math.h>
#include <QThread>

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

    // A signal that the image needs to be redrawn when the window is resized
    connect(paintWidget, SIGNAL(canvas_resized(QSize, QSize)), this, SLOT(canvas_resized(QSize, QSize)));

    // Thread creation
    QThread* thread = new QThread;
    Worker* worker = new Worker(&alg, &Thread_status, p, l);

    // We transfer the ownership rights of the workerclass to the QThread class
    worker->moveToThread(thread);

    // We connect the "started" signal of the thread with the "process" slot of the worker-class,
    // the execution of calculations begins
    connect(thread, SIGNAL(started()), worker, SLOT(process()));

    // Communication of the main Gui thread with the secondary thread
    connect(worker, SIGNAL(SendStep(int)), this, SLOT(recieve_step(int)));

    // Signals that are sent to the stream
    // Signal processing of buttons and input fields
    connect(this, SIGNAL(send_delete_thread()), worker, SLOT(RecieveDeleteThread()), Qt::DirectConnection);
    connect(ui->RunButton, SIGNAL(clicked()), worker, SLOT(RecieveRun()), Qt::DirectConnection);
    connect(ui->StopButton, SIGNAL(clicked()), worker, SLOT(RecievePause()), Qt::DirectConnection);
    connect(ui->ChangeAlgoButton, SIGNAL(clicked()), worker, SLOT(RecieveChangeAlgo()), Qt::DirectConnection);
    connect(ui->BetaSpinBox, SIGNAL(valueChanged(double)), worker, SLOT(RecieveNewBeta(double)), Qt::DirectConnection);
    connect(ui->ChangeAlgoButton, SIGNAL(clicked()), this, SLOT(change_algo_label()));

    // Upon completion, we exit the stream, and delete the worker-class
    connect(worker, SIGNAL(destroyed(QObject*)), thread, SLOT(quit()));  // THE PROPER WAY
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    // Delete the stream after performing the calculations
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    Thread_status = true;
    thread->start();
}

//destructor - free resources
MainWindow::~MainWindow()
{
    send_delete_thread();
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

void MainWindow::recieve_step(int number)
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

void MainWindow::change_algo_label()
{
	if (alg == 0)
        lb3->setText("Heat bath algorithm");
	if (alg == 1)
		lb3->setText("Clusters algorithm");
}

//called when form and widget is resized, repaints widget
void MainWindow::canvas_resized(QSize old_size, QSize new_size)
{
	if (old_size != new_size)
		draw_picture();
}
