#include "hdr/mainwindow.h"
#include <QApplication>
#include <QTextStream>
#include <fstream>
#include <iostream>
#include "hdr/funcs.h"
#include <stdlib.h>
#include <string.h>

int usage() {
	std::cout << "Usage: ising_model [options]\n"
				 "or     ising_model -plot input output [plot options]" << std::endl;
	std::cout << "For more information use -help option" << std::endl;
	return 0;
}

int help() {
	std::cout << "For simulation with GUI: ising_model [options]" << std::endl;
	std::cout << "options:\n-lsize=<number> [default: 64]\n"
				 "\tSets lattice size\n" << std::endl;
	std::cout << "For making average magnetization from beta plot (in console): ising_model -plot input output [plot options]\n"
				 "in input file there should be number of points n at first (n > 0), and then n beta values!" << std::endl;
	std::cout << "plot options:\n"
				 "-lsize=<number> [default: 64]\n"
				 "-algo=<1|2> [default: 1]\n"
				 "\tChooses simulation algorithm:\n"
				 "\t1 - Heat bath algorithm\n"
				 "\t2 - Clasters algorithm\n"
				 "-steps=<number> [default: 100]\n"
				 "\tSets number of algorithm steps\n"
				 "-avg=<number> [default: 1]\n"
				 "\tSets averaging by results for each plot point" << std::endl;
	return 0;
}

int main(int argc, char *argv[]) {
	//for help
	if (argc > 1 && strcmp(argv[1], "-help") == 0)
		return help();

	int lsize = 64; //default lattice size
	//for making plot
	if (argc > 1 && strcmp(argv[1], "-plot") == 0) {
		if (argc < 4) {
			std::cout << "-plot: missing input and output files" << std::endl;
			return usage();
		}
		int algo = 1, steps = 100, averaging = 1;
		//parsing plot options
		for (int arg = 4; arg < argc; arg++) {
			if (strstr(argv[arg], "-lsize=") == argv[arg])
				lsize = atoi(argv[arg] + strlen("-lsize="));
			else if (strstr(argv[arg], "-algo=") == argv[arg])
				algo = atoi(argv[arg] + strlen("-algo="));
			else if (strstr(argv[arg], "-steps=") == argv[arg])
				steps = atoi(argv[arg] + strlen("-steps="));
			else if (strstr(argv[arg], "-avg=") == argv[arg])
				averaging = atoi(argv[arg] + strlen("-avg="));
			else {
				std::cout << "unrecognized option: " << argv[arg] << std::endl;
				return usage();
			}
			//std::cout << lsize << " " << steps << " " << averaging << std::endl;
		}
		//checking plot parameters
		if (lsize <= 0) {
			std::cout << "-lsize: incorrect lattice size (must be > 0)" << std::endl;
			return 0;
		}
		if (algo != 1 && algo != 2) {
			std::cout << "-algo: invalid algorithm id (must be 1 or 2), see help" << std::endl;
			return 0;
		}
		if (steps <= 0) {
			std::cout << "-steps: incorrect number of steps (must be > 0)" << std::endl;
			return 0;
		}
		if (averaging <= 0) {
			std::cout << "-avg: incorrect averaging value (must be > 0)" << std::endl;
			return 0;
		}
		//std::cout << "plot making" << std::endl;
		return calc_plot(argv[2], argv[3], lsize, algo - 1, steps, averaging);
	}

	//for opening window with simulation
	if (argc > 1) {
		if (strstr(argv[1], "-lsize=") == argv[1])
			lsize = atoi(argv[1] + strlen("-lsize="));
		else {
			std::cout << "unrecognized option: " << argv[1] << std::endl;
			return usage();
		}
		if (lsize <= 0) {
			std::cout << "-lsize: incorrect lattice size (must be > 0)" << std::endl;
			return 0;
		}
		std::cout << "simulation with " << lsize << "x" << lsize << " lattice" << std::endl;
	}
	QApplication a(argc, argv);
	MainWindow w(lsize);
	w.show();
	return a.exec();
}
