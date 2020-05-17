#include "mainwindow.h"
#include <QApplication>
#include <fstream>
#include "funcs.h"
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc <= 2) {
		QApplication a(argc, argv);
		MainWindow w;
		w.show();
		//if (argc) w.set_alg(
		return a.exec();
	}
    else {
		calc_plot(argv[1], argv[2]);
	}
	return 0;
}
