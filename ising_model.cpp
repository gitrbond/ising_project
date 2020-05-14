#include "ising_model.h"
#include <assert.h>
#include <math.h>
#include <fstream>

parameters::parameters(double beta, double H, double J, double mu) :
    beta(beta), H(H), J(J), mu(mu) {
#ifdef DEBUG
        cout << "parameters()" << endl;
#endif
}

void parameters::set_beta(const double new_beta) {
        beta = new_beta;
}

parameters::~parameters() {
#ifdef DEBUG
    cout << "~parameters()" << endl;
#endif
}

Monte_Carlo::Monte_Carlo(parameters &p) : parameters(p) {
#ifdef DEBUG
    cout << "Monte_Carlo()" << endl;
#endif
}

void Monte_Carlo::simulate(lattice *l, int steps) const {
    int nbrs = l->getnbrs();
	int *prob_arr = new int [1 + l->getnbrs()]; //array of all possible probabilities
	for (int i = 0; i <= nbrs; i++)
		prob_arr[i] =  round(RAND_MAX / (1 + exp(-2 * beta *(2 * i - nbrs) - mu * H)));
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

void Monte_Carlo::clasters_simulate(lattice *l, int steps) const {
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
            vdel(Pocket, spin); //delete from pocket
        }
        for (auto i = Claster.begin(), end = Claster.end(); i != end; ++i)
            l->getL()[*i] = -l->getL()[*i]; //flipping claster
		if (int(Claster.size()) > 5 * l->getN() / 6) //cuts unnecessary calculations
			break;
    }
    delete [] nbr_arr;
}

int Monte_Carlo::def_spin(int plus_prob) const {
    int rand_prob = rand();
    if (rand_prob < plus_prob)
        return 1;
    return -1;
}

void Monte_Carlo::plot_magn_beta(lattice *l, const vector <double> &beta_points, vector <double> &magn_points, const int steps, const int averaging) { //the 5th version
	assert(averaging > 0);

	const char *filename = "data_magn_beta";
	ofstream output(filename);
	if (output) {
		magn_points.clear();
		for (auto i = beta_points.begin(); i != beta_points.end(); ++i) {
			beta = *i;
			double avg_magn = 0;

			for(int j = 0; j < averaging; j++) {
				l->fill_random();
				simulate(l, steps);
				double mes;
				mes = abs(l->avg_magn());
				output << mes << " ";
				avg_magn += mes;
				cout << ".";
			}
			avg_magn /= averaging;
			magn_points.push_back(avg_magn);
			output << endl << beta << " " << magn_points.back() << endl;
			cout << magn_points.back() << endl;
		}
		output << endl;

		for (auto i = magn_points.begin(); i != magn_points.end(); ++i)
			output << *i << endl;
	}
	else
		cerr << "Unable to open output file " << filename << endl;
}

void Monte_Carlo::test(lattice *l) {//test here
    l->fill_random();
    cout << "step 0:" << endl;
    l->show();
    cout << "avg. magn = " << l->avg_magn() << endl;

    int steps = 300;
    simulate(l, steps);
    //clasters_simulate(l);
    cout << "step " << steps << ":" << endl;
    l->show();
    cout << "avg. magn = " << l->avg_magn() << endl;
}

/*int main() {
    srand((unsigned)time(NULL));
    parameters p(0.55); //beta
    square_lattice *l = new square_lattice(64, 64);
    Monte_Carlo model(p);
    model.test(l);
    delete l;
    return 0;
}*/
