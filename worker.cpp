#include "worker.h"
//#include <windows.h>
#include <QThread>
#include <QDebug>
#include <QPainter>

Worker::Worker(bool *Status, parameters p, lattice *lptr, int *num_alg, QObject *parent) :
    QObject(parent), Thread_status(Status), l(lptr), model(new Monte_Carlo(p)), num_alg(num_alg)
{
    Stop = false;
    Run = false;
    step = 0;
}

Worker::~Worker()
{
    qDebug() << "destruction Thread";
    delete model;
    //qDebug() << "Send Thread deleted";
    *Thread_status = false;
}

void Worker::process()
{
    Stop = false;
    Run = false;
    while (!Stop)
    {
        if(Run && !Stop)
        {
            if (*num_alg == 1)
                model->simulate(l);
            if (*num_alg == -1)
                model->clasters_simulate(l);
            step++;
            emit(sendNumber(step));
            QThread::msleep(50);
        }
        else
            QThread::msleep(100);
    }
    emit finished(); // вызывается при завершении расчёта
}

void Worker::RecieveDeleteThread()
{
    Stop = true;
    qDebug() << "RecieveDeleteThread = " << Stop;
    emit finished(); // вызывается при отмене расчёта
}

void Worker::Recieve_change_alg()
{
    *num_alg = - *num_alg;
    qDebug() << "Algo changed to " << *num_alg;
}

void Worker::RecieveRun()
{
    Run = true;
    //qDebug() << "Recieve Run";
}

void Worker::RecievePause()
{
    Run = false;
    //qDebug() << "Recieve Pause";
}

void Worker::RecieveNewBeta(double new_beta)
{
    //qDebug() << "Recieved " << new_beta << " beta";
    model->set_beta(new_beta);
}
