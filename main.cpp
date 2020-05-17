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
        if (argc == 2) {
            int num = (argv[1])[0] - '0';
            if (num == 1)
                w.choose_alg(1);
            if (num == 2)
                w.choose_alg(-1);
        }
        w.show();
		return a.exec();
	}
    else {
		calc_plot(argv[1], argv[2]);
	}
	return 0;
}
