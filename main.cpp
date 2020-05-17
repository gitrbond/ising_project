#include "mainwindow.h"
#include <QApplication>
#include <fstream>

int main(int argc, char *argv[])
{
	if (argc == 1) {
		QApplication a(argc, argv);
		MainWindow w;
		w.show();

		return a.exec();
	}
	
	ifstream input(argv[1]);
	if (input) {
		const char *str;
        if (argc > 2) {
            str = argv[2];
        } else {
            str = "output.txt";
        }

		ofstream output(str);
		if (output) {
			srand((unsigned)time(NULL));
		
			unsigned n;
			input >> n;
			vector < double > beta_points(n), magn_points(n);
			for (unsigned i = 0; i < n; i++) {
				input >> beta_points[i];
			}

			int steps = 200;
			int averaging = 20;
			parameters p(0.5);
			square_lattice *l = new square_lattice(64);

			Monte_Carlo model(p);
			model.plot_magn_beta(l, beta_points, magn_points, steps, averaging);

			delete l;
			for (unsigned i = 0; i < n; i++) {
				output << magn_points[i] << endl;
			}
		} else {
			cout << "Cannot open outputfile." << endl;
		}
	} else {
		cout << "Cannot open inputfile." << endl;
	}
	return 0;
}
