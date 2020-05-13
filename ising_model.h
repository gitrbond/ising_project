#ifndef ISING_MODEL_H
#define ISING_MODEL_H

#include <iostream>
#include <vector> //essential
#include <stdlib.h>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <assert.h>
#include <lattice.h>

//#define DEBUG

#ifdef DEBUG
#include "debug.h" //debug functions
#endif

using namespace std;

//inline bool vcontains(const vector <int> &v, int el);
//bool vdel(vector <int> &v, int el);

class parameters {
protected:
    //double T; //temperature in Kelvins - needs later
    double beta; //beta = 1/kT
    double H; //outer magnetic field
    double J; //exchange energy
    double mu; //magnetic moment

public:
    parameters(double beta, double H = 0, double J = 1, double mu = 1) : beta(beta), H(H), J(J), mu(mu) {
#ifdef DEBUG
        cout << "parameters()" << endl;
#endif
    }

    void set_beta(const double new_beta) {
            beta = new_beta;
    }

    virtual ~parameters() {
#ifdef DEBUG
        cout << "~parameters()" << endl;
#endif
    }
};

class Monte_Carlo : public parameters {
public:
    Monte_Carlo (parameters &p) : parameters(p) {
#ifdef DEBUG
        cout << "Monte_Carlo()" << endl;
#endif
    }

    void simulate(lattice *l, int steps) const {
        int nbrs = l->getnbrs();
        int *prob_arr = new int [1 + l->getnbrs()]; //array of all possible probabilities
        for (int i = 0; i <= nbrs; i++)
            prob_arr[i] =  round(RAND_MAX / (1 + exp(-2 * beta *((2 * i - nbrs) + mu * H))));
        for (int i = 0; i < steps; i++) {
            for (int j = 0; j < l->getN(); j++) {
                int rand_spin = big_rand() % l->getN();
                assert((nbrs + l->sum_nbr(rand_spin)) / 2 < 1 + nbrs);
                int prob = prob_arr[(nbrs + l->sum_nbr(rand_spin)) / 2];
                l->getL()[rand_spin] = def_spin(prob);
            }
        }
        delete [] prob_arr;
    }

    void clasters_simulate(lattice *l) const {
        int steps = 10 * sqrt(l->getN()) * exp(-3 * beta);
        int prob = RAND_MAX * (1 - exp(-2 * beta)); //magical number
        int *nbr_arr = new int[l->getnbrs()];
        for (int j = 0; j < steps; j++) {
            int spin = big_rand() % l->getN();
            vector <int> Claster {spin}, Pocket {spin};
            while (Pocket.size()) {
                spin = Pocket[big_rand() % Pocket.size()]; //randomly choose from pocket
                l->get_nbrs(spin, nbr_arr);
                for (int i = 0; i < l->getnbrs(); i++) {
                    if (rand() < prob && //take spin with probability
                        l->getL()[spin] == l->getL()[nbr_arr[i]] && !vcontains(Claster, nbr_arr[i])) {
                        Pocket.push_back(nbr_arr[i]); //add it to pocket
                        Claster.push_back(nbr_arr[i]); //and to claster
                    }
                }
                assert(vdel(Pocket, spin)); //delete from pocket
#ifdef DEBUG
                Dshow("Pocket", Pocket);
                Dshow("Claster", Claster);
#endif
            }
            for (auto i = Claster.begin(), end = Claster.end(); i != end; ++i)
                l->getL()[*i] = -l->getL()[*i]; //flipping claster
        }
        delete [] nbr_arr;
    }

    int def_spin(int plus_prob) const {
        int rand_prob = rand();
        if (rand_prob < plus_prob)
            return 1;
        return -1;
    }

    void test(lattice *l);
};

#endif // ISING_MODEL_H
