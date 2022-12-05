#include "hdr/ising_model.h"
#include "hdr/funcs.h"
#include <assert.h>
#include <math.h>
#include <fstream>

parameters::parameters(double beta, double H, double J, double mu) :
    beta(beta), H(H), J(J), mu(mu) {
}

void parameters::set_beta(const double new_beta) {
    beta = new_beta;
}

parameters::~parameters() {
}

MonteCarlo::MonteCarlo(parameters &p) : parameters(p) {
}

void MonteCarlo::heat_bath_simulate(lattice *l, int steps) const {
    int rand_spin, prob, nbrs = l->getnbrs(), *L = l->getL(), N = l->getN();
    int prob_arr[1 + 2 * nbrs]; // Array of all possible probabilities
    for (int i = -nbrs; i <= nbrs; ++i) // Filling the array
        prob_arr[i + nbrs] = RAND_MAX / (1 + exp(-2 * beta * i - mu * H));
    for (int i = 0; i < steps; ++i) {
        for (int j = 0; j < N; ++j) {
            rand_spin = rand_30bit() % N; // Choose an arbitrary spin
            prob = prob_arr[l->sum_nbr(rand_spin) + nbrs]; // Take the previously calculated probability from the array
            L[rand_spin] = def_spin(prob); // Assign +1 or -1
        }
    }
}

void MonteCarlo::clusters_simulate(lattice *l, int steps) const {
    int spin, *L = l->getL(), N = l->getN();
    unsigned int nbrs = l->getnbrs(), nbr_arr[nbrs];
    int prob = RAND_MAX * (1 - exp(-2 * beta)); // The magic number
    for (int j = 0; j < steps; ++j) {
        spin = rand_30bit() % N; 						// Arbitrary choice of spin
        std::vector <int> Cluster {spin}, Pocket {spin}; // Put the spin in the cluster and in the pocket
        while (!Pocket.empty()) {
            spin = Pocket[rand_30bit() % Pocket.size()]; 	// An arbitrary choice from the pocket
            l->get_nbrs(spin, nbr_arr); 				// Get neighbors spin
            for (unsigned int i = 0; i < nbrs; ++i) { 			// Check all the neighbors:
                if (L[spin] == L[nbr_arr[i]] && 			// If the neighbor's spin is the same
                    !vcontains(Cluster, nbr_arr[i]) && 		// and it's not in the cluster yet,
                    rand() < prob) { 							// then with probability prob
                    Pocket.push_back(nbr_arr[i]); 				// Add to Pocket
                    Cluster.push_back(nbr_arr[i]);				// Add to Cluster
                }
            }
            vdel(Pocket, spin); 						// Remove from pocket
        }
        for (auto i = Cluster.begin(); i != Cluster.end(); ++i)
            L[*i] = - L[*i]; 						// Flip the whole cluster
        if (Cluster.size() > 5 * l->getN() / 6) // Cuts unnecessary calculations
            break;
    }
}

int MonteCarlo::def_spin(int plus_prob) const {
    int rand_prob = rand();
    if (rand_prob < plus_prob)
        return 1;
    return -1;
}

void MonteCarlo::plot_magn_beta(lattice *l, const std::vector <double> &beta_points, std::vector <double> &magn_points, const int steps, const int averaging, const int algo) {
    try {
        if (averaging <= 0)
			throw Exception("averaging must be positive, you entered: ", averaging);

		magn_points.clear();
		std::cout << "Plot of the function average magnetization from temperature beta, avg_magn(beta)" << std::endl;
		if (algo == 0)
			std::cout << "Heat bath algorithm" << std::endl;
		if (algo == 1)
			std::cout << "Clasters algorithm" << std::endl;
		for (auto i = beta_points.begin(); i != beta_points.end(); ++i) {
			beta = *i;
			double avg_magn = 0;
			double mes;
			for(int j = 0; j < averaging; j++) {
				l->fill_random();
				if (algo == 0)
					heat_bath_simulate(l, steps);
				if (algo == 1)
					clusters_simulate(l, steps);
				mes = l->avg_magn();
				avg_magn += abs(mes);
			}
			avg_magn /= averaging;
			magn_points.push_back(avg_magn);
			std::cout << "beta = " << beta << "\tavg_magn = " << magn_points.back() << std::endl;
		}
	}
	catch (Exception &exc) {
        std::cout << exc.what() << exc.Get_data() << std::endl;
    }
}

void MonteCarlo::test(lattice *l) {//test here
    l->fill_random();
    std::cout << "step 0:" << std::endl;
    l->show();
    std::cout << "avg. magn = " << l->avg_magn() << std::endl;

    int steps = 300;
	heat_bath_simulate(l, steps);
	//clusters_simulate(l);
    std::cout << "step " << steps << ":" << std::endl;
    l->show();
    std::cout << "avg. magn = " << l->avg_magn() << std::endl;
}

int simulation_example() {
    srand((unsigned)time(NULL));
    parameters p(0.55); //beta
    square_lattice *l = new square_lattice(10);
    MonteCarlo model(p);
    model.test(l);
    delete l;
    return 0;
}
