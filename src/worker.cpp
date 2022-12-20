#include "hdr/worker.h"
#include <QThread>
#include <QDebug>
#include <QPainter>


Worker::Worker(int* alg, bool *status, Parameters p, Lattice *lptr, QObject *parent) :
QObject(parent), alg(alg), threadStatus(status), l(lptr), model(new MonteCarlo(p))

{
  stop = false;
  run = false;
  step = 0;
}

Worker::~Worker()
{
  //qDebug() << "destruction Thread";
  delete model;
  *threadStatus = false;
}

void Worker::process()
{
  stop = false;
  run = false;
  while (!stop)
  {
    if(run && !stop)
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
  stop = true;
  //qDebug() << "receiveDeleteThread = " << Stop;
  emit finished();
}

void Worker::receiveChangeAlgo()
{
  *alg = (*alg + 1) % 2;
  //qDebug() << "Algo changed to " << *alg;
}

void Worker::receiveRun()
{
  run = true;
  //qDebug() << "Recieve Run";
}

void Worker::receivePause()
{
  run = false;
  //qDebug() << "Recieve Pause";
}

void Worker::receiveNewBeta(double newBeta)
{
  //qDebug() << "Recieved " << new_beta << " beta";
  model->setBeta(newBeta);
}