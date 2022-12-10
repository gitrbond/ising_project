#include "hdr/worker.h"
#include <QThread>
#include <QDebug>
#include <QPainter>

worker::worker(int* alg, bool *Status, parameters p, lattice *lptr, QObject *parent) :
    QObject(parent), alg(alg), Thread_status(Status), l(lptr), model(new monteCarlo(p))

{
    Stop = false;
    Run = false;
    step = 0;
}

worker::~worker()
{
    //qDebug() << "destruction Thread";
	delete model;
    *Thread_status = false;
}

void worker::process()
{
    Stop = false;
    Run = false;
    while (!Stop)
    {
        if(Run && !Stop)
        {
			if (*alg == 0)
				model->heatBathSimulate(l);
			if (*alg == 1)
				model->clustersSimulate(l);
            step++;
			emit(sendStep(step));
            QThread::msleep(50);
        }
        else
            QThread::msleep(100);
    }
    emit finished();
}

void worker::receiveDeleteThread()
{
    Stop = true;
	//qDebug() << "receiveDeleteThread = " << Stop;
    emit finished();
}

void worker::receiveChangeAlgo()
{
	*alg = (*alg + 1) % 2;
	//qDebug() << "Algo changed to " << *alg;
}

void worker::receiveRun()
{
    Run = true;
    //qDebug() << "Recieve Run";
}

void worker::receivePause()
{
    Run = false;
    //qDebug() << "Recieve Pause";
}

void worker::receiveNewBeta(double new_beta)
{
    //qDebug() << "Recieved " << new_beta << " beta";
    model->setBeta(new_beta);
}
