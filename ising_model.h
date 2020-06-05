#ifndef ISING_MODEL_H
#define ISING_MODEL_H

#include <iostream>
#include "lattice.h"
#include <vector>

//#define DEBUG

#ifdef DEBUG
#include "debug.h" //debug functions
#endif

using namespace std;

class parameters {
protected:
    double beta; //beta = 1/kT
    double H; //outer magnetic field
    double J; //exchange energy
    double mu; //magnetic moment

public:
	parameters(double beta = 0.4, double H = 0, double J = 1, double mu = 1);
    virtual ~parameters();

    void set_beta(const double new_beta);
};

class Monte_Carlo : public parameters {
public:
    Monte_Carlo(parameters &p);

	void heat_bath_simulate(lattice *l, int steps = 1) const;
    void clasters_simulate(lattice *l, int steps = 1) const;
    int def_spin(int plus_prob) const;
	void plot_magn_beta(lattice *l, const vector <double> &beta_points, vector <double> &magn_points, const int steps, const int averaging, const int algo);
    void test(lattice *l);
};

#endif // ISING_MODEL_H
