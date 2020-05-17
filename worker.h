#ifndef WORKER_H
#define WORKER_H

#include <QMainWindow>
#include <QObject>
#include <QPainter>
#include "paintwidget.h"
#include "ising_model.h"

namespace Ui {
class MainWindow;
}

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(bool *Status, parameters p, lattice *lptr, int *num_alg, QObject *parent = 0);
    ~Worker();

    bool *Thread_status;
    bool Stop;
    bool Run;
    int step;
    lattice *l;
    Monte_Carlo *model;
    int *num_alg;

signals:
    void finished();

    void sendNumber(int);

public slots:
    void process();

    void RecieveDeleteThread();
    void Recieve_change_alg();
    void RecievePause();
    void RecieveRun();
    void RecieveNewBeta(double new_beta);
};

#endif // WORKER_H
