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
    int rand_spin, prob, nbrs = l->getnbrs(), *L = l->getL(), N = l->getN();
	int prob_arr[1 + 2 * nbrs]; // Массив всех возможных вероятностей
	for (int i = -nbrs; i <= nbrs; ++i) // Заполнение массива
		prob_arr[i + nbrs] = RAND_MAX / (1 + exp(-2 * beta * i - mu * H));
	for (int i = 0; i < steps; ++i) {
		for (int j = 0; j < N; ++j) {
			rand_spin = big_rand() % N; // Выбрать произвольный спин
			prob = prob_arr[l->sum_nbr(rand_spin) + nbrs]; // Взять высчитанную ранее вероятность
			L[rand_spin] = def_spin(prob); // Присвоить +1 или -1
		}
	}
}

void Monte_Carlo::clasters_simulate(lattice *l, int steps) const {
    int spin, *L = l->getL(), N = l->getN();
    unsigned int nbrs = l->getnbrs(), nbr_arr[nbrs];
	int prob = RAND_MAX * (1 - exp(-2 * beta)); // Магическое число
	for (int j = 0; j < steps; ++j) {
		spin = big_rand() % N; 						// Произвольный выбор спина
		vector <int> Claster {spin}, Pocket {spin}; // Кладем его в кластер и в карман
		while (!Pocket.empty()) {
			spin = Pocket[big_rand() % Pocket.size()]; 	// Произвольный выбор из кармана
			l->get_nbrs(spin, nbr_arr); 				// Получить соседей спина
			for (unsigned int i = 0; i < nbrs; ++i) { 			// Проверить всех соседей:
				if (L[spin] == L[nbr_arr[i]] && 			// Если спин соседа совпадает
					!vcontains(Claster, nbr_arr[i]) && 		// и его еще нет в кластере,
					rand() < prob) { 							// то с вероятностью prob
					Pocket.push_back(nbr_arr[i]); 				// Добавление в карман
					Claster.push_back(nbr_arr[i]);				// Добавление в кластер
				}
			}
			vdel(Pocket, spin); 						// Удалить из кармана
		}
		for (auto i = Claster.begin(); i != Claster.end(); ++i)
			L[*i] = - L[*i]; 						// Переворот кластера
		if (Claster.size() > 5 * l->getN() / 6) //cuts unnecessary calculations
			break;
    }
}

int Monte_Carlo::def_spin(int plus_prob) const {
    int rand_prob = rand();
    if (rand_prob < plus_prob)
        return 1;
    return -1;
}

//start working
void Monte_Carlo::plot_magn_beta(lattice *l, const vector < double > &beta_points, vector < double > &magn_points, const int steps, const int averaging) {
    try {
        if (averaging <= 0)
            throw Exception("Averaging must be positive, you entered: ", averaging);

		magn_points.clear();

		for (auto i = beta_points.begin(); i != beta_points.end(); ++i) {
			beta = *i;
			double avg_magn = 0;
			double mes;
			for(int j = 0; j < averaging; j++) {
				l->fill_random();
				clasters_simulate(l, steps);
				mes = l->avg_magn();
				avg_magn += mes*mes;
			}
			avg_magn /= averaging;
			magn_points.push_back(avg_magn);
			cout << magn_points.back() << endl;
		}
	}
	catch (Exception &exc) {
        cout << exc.what() << exc.Get_data() << endl;
    }
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

#if 0
int main() {
    srand((unsigned)time(NULL));
    parameters p(0.55); //beta
    square_lattice *l = new square_lattice(0);
    Monte_Carlo model(p);
    model.test(l);
    delete l;
    return 0;
}
#endif
