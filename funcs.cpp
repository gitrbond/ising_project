#include "funcs.h"
#include "lattice.h"
#include "ising_model.h"
#include <algorithm>
#include <fstream>
#include <ctime>
#include <iostream>

bool vcontains(const vector <int> &v, int el) { //checks if vector contains element
    return find(v.begin(), v.end(), el) != v.end();
}

bool vdel(vector <int> &v, int el) { //deletes element from vector. returns false if not found
    auto del = find(v.begin(), v.end(), el);
    if (del != v.end()) {
        v.erase(del);
        return true;
    }
    return false;
}

void vlinfill(vector <double> &v, const double start, const double end, const int points) {
	for (int i = 0; i < points; i++)
		v.push_back(start + i * (end - start) / (points - 1));
}

int big_rand() { //30-bit random number
    int r = rand() & 0x7FFF;
    r = (r << 15) | (rand() & 0x7FFF);
    return r;
}

void calc_plot(const char* f_input, const char* f_output) {
    std::cout << f_input << " " << f_output;
	ifstream input(f_input);
    if (input) {
		ofstream output(f_output);
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
		}
		else {
			cout << "Cannot open outputfile." << endl;
		}
	}
	else {
		cout << "Cannot open inputfile." << endl;
	}
}

