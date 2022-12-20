#ifndef WORKER_H
#define WORKER_H

#include "hdr/ising_model.h"
#include <QMainWindow>
#include <QObject>
#include <QPainter>

namespace Ui {
class MainWindow;
}

class worker : public QObject
    {
  Q_OBJECT

     public:
      explicit worker(int* alg, bool *Status, parameters p, lattice *lptr = nullptr, QObject *parent = 0);
  ~worker();

  int* alg;
  bool *Thread_status;
  bool Stop;
  bool Run;
  int step;
  lattice *l;
  monteCarlo *model;

  signals:
      void finished();

      void sendStep(int);

      public slots:
      void process();

      void receiveDeleteThread();
      void receiveChangeAlgo();
      void receivePause();
      void receiveRun();
      void receiveNewBeta(double new_beta);
    };

#endif // WORKER_H

