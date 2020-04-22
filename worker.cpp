#include "worker.h"
#include <windows.h>
#include <QDebug>
#include <QPainter>

Worker::Worker(QObject *parent) : QObject(parent)
{
    Stop = false;
    temp = 0;
    l = nullptr;
    model = nullptr;
}

Worker::~Worker()
{
    qDebug() << "destruction Thread";
}

void Worker::process()
{
    Stop = false;
    while ((model == nullptr || l == nullptr) && !Stop) {
        qDebug() << "waiting for model...";
        Sleep(500);
    }
    if(!Stop)
    {
        for (; temp <= 15; temp++)
        {
            if(!Stop == true)
            {
            model->simulate(l, 1);
            emit(sendNumber(temp));
            qDebug() << "simulating, temp= " << temp;
            Sleep(100);
            }
            else
            {
                return;
            }
        }
        /*QPainter painter(paintWidget->image);
        painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
        painter.setPen(Qt::black);
        int w = paintWidget->width(), h = paintWidget->height();
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if(!Stop == true) {
                    qDebug() << "i work ";
                    painter.drawEllipse(i*w/5, j*h/5, w/5, h/5);
                    Sleep(1000);
                }
                else
                    return;
            }
        }*/
    }
    emit finished(); // вызывается при завершении расчёта
}

void Worker::reciveBoolStop(bool Numb)
{
    Stop = Numb;
    qDebug() << "reciveBoolStop = " << Stop;
    emit finished(); // вызывается при отмене расчёта
}

void Worker::Recieve_model(parameters p, lattice *lptr)
{
    qDebug() << "lattice recieved";
    model = new Monte_Carlo(p);
    l = lptr;
}
