#include "worker.h"
#include <windows.h>
#include <QDebug>
#include <QPainter>

Worker::Worker(parameters p, lattice *lptr, QObject *parent) :
    QObject(parent), l(lptr), model(new Monte_Carlo(p))
{
    Stop = false;
    Run = false;
    step = 0;
}

Worker::~Worker()
{
    qDebug() << "destruction Thread";
    delete model;
}

void Worker::process()
{
    Stop = false;
    Run = false;
    /*while ((model == nullptr || l == nullptr) && !Stop) {
        qDebug() << "waiting for model...";
        Sleep(500);
    }*/
    while (!Stop)
    {
        if(Run && !Stop)
        {
        model->simulate(l, 1);
        step++;
        emit(sendNumber(step));
        //qDebug() << "simulating, temp= " << step;
        Sleep(50);
        }
        else
            Sleep(100);
    }
    emit finished(); // вызывается при завершении расчёта
}

void Worker::RecieveDeleteThread()
{
    Stop = true;
    qDebug() << "RecieveDeleteThread = " << Stop;
    emit finished(); // вызывается при отмене расчёта
}

void Worker::RecieveRun()
{
    Run = true;
    qDebug() << "Recieve Run";
}

void Worker::RecievePause()
{
    Run = false;
    qDebug() << "Recieve Pause";
}

/*void Worker::Recieve_model(parameters p, lattice *lptr)
{
    qDebug() << "lattice recieved";
    //model = new Monte_Carlo(p);
    //l = lptr;
}*/
