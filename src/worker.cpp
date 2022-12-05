#include "hdr/worker.h"
#include <QThread>
#include <QDebug>
#include <QPainter>

Worker::Worker(int* alg, bool *Status, parameters p, lattice *lptr, QObject *parent) :
    QObject(parent), alg(alg), Thread_status(Status), l(lptr), model(new MonteCarlo(p))

{
    Stop = false;
    Run = false;
    step = 0;
}

Worker::~Worker()
{
    //qDebug() << "destruction Thread";
	delete model;
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
			if (*alg == 0)
				model->heat_bath_simulate(l);
			if (*alg == 1)
				model->clusters_simulate(l);
            step++;
			emit(SendStep(step));
            QThread::msleep(50);
        }
        else
            QThread::msleep(100);
    }
    emit finished();
}

void Worker::RecieveDeleteThread()
{
    Stop = true;
	//qDebug() << "RecieveDeleteThread = " << Stop;
    emit finished();
}

void Worker::RecieveChangeAlgo()
{
	*alg = (*alg + 1) % 2;
	//qDebug() << "Algo changed to " << *alg;
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
