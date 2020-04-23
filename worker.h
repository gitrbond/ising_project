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
    explicit Worker(parameters p, lattice *lptr = nullptr, QObject *parent = 0);
    ~Worker();

    bool Stop;
    bool Run;
    int step;
    lattice *l;
    Monte_Carlo *model;

signals:
    void finished();
    //void error(QString err);

    void sendNumber(int);

public slots:
    void process();

    void RecieveDeleteThread();
    void RecievePause();
    void RecieveRun();
    //void Recieve_model(parameters p, lattice *lptr);

private:

};

#endif // WORKER_H
