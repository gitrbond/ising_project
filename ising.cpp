#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctime>

using namespace std;

int big_rand();

class lattice { //abstract
protected:
	int N; //number of spins
	int *L; //spins in array
	int nbrs; //nuber of nbrs

public:
	lattice(int N, int nbrs) : N(N), L(new int[N]), nbrs(nbrs) {
		cout << "lattice(" << N << ")" << endl;
	}

	void fill_random() {
		//srand(time(0));
		for (int i = 0; i < N; i++)
			L[i] = 2 * (rand() % 2) - 1;
	}

	virtual int sum_nbr(int index) = 0; //returns sum of neighbour spins

	virtual void show() = 0; //the pure virtual function

	virtual ~lattice() {
		delete [] L;
		cout << "~lattice()" << endl;
	}

	double avg_magn() {//returns average magnetization
		int sum = 0;
		for (int i = 0; i < N; i++)
			sum += L[i];
		return (double) sum / N;
	}

	friend class Monte_Carlo;
};

class square_lattice : public lattice {
	int A, B; //lattice sizes: A strings, B columns

public:
	square_lattice(int A, int B) : lattice(A * B, 4), A(A), B(B) {
		cout << "sq_lattice(" << A << "*" << B << ")" << endl;
	}

	int sum_nbr(int index) {
		int a = index / B, b = index % B;
		return L[B * ((a + A - 1) % A) + b] +
		+ L[B * ((a + 1) % A) + b] +
		+ L[B * a + (b + 1) % B] +
		+ L[B * a + (b + B - 1) % B];
	}

	void show() {
		for (int i = 0; i < A; i++) {
			for (int j = 0; j < B; j++)	{
				if (L[B * i + j] > 0)
					cout << "+"; //+1
				else
					cout << " "; //-1
			}
			cout << endl;
		}
	}

	~square_lattice() {
		cout << "~sq_lattice()" << endl;
	}
};

class parameters {
protected:
	//double T; //temperature in Kelvins - needs later
	double beta; //beta = 1/kT
	double H; //outer magnetic field
	double J; //exchange energy
	double mu; //magnetic moment

public:
	parameters(double beta, double H = 0, double J = 1, double mu = 1) : beta(beta), H(H), J(J), mu(mu) {
		cout << "parameters()" << endl;
	}

	parameters(const parameters &p) : beta(p.beta), H(p.H), J(p.J), mu(p.mu) {
		cout << "parameters() copy constructor" << endl;
	}

	virtual ~parameters() {
		cout << "~parameters()" << endl;
	}
};

class Monte_Carlo : public parameters {
	lattice *l;
	int *prob_arr; //array of all possible probabilities

public:
	Monte_Carlo (parameters p, lattice *l) : parameters(p), l(l), prob_arr(new int [1 + l->nbrs]) {
		/*for (int i = 0; i <= l->nbrs; i++)
			prob_arr[i] =  round(RAND_MAX / (1 + exp(-2 * beta *((2 * i - l->nbrs) + mu * H))));*/
		cout << "Monte_Carlo()" << endl;
	}

	void simulate(int steps) {
		for (int i = 0; i <= l->nbrs; i++)
                        prob_arr[i] =  round(RAND_MAX / (1 + exp(-2 * beta *((2 * i - l->nbrs) + mu * H))));
		for (int i = 0; i < steps; i++) {
			for (int j = 0; j < l->N; j++) {
				int rand_spin = big_rand() % l->N;
				int prob = prob_arr[(l->nbrs + l->sum_nbr(rand_spin)) / 2];
				l->L[rand_spin] = def_spin(prob);
			}
		}
	}

	int def_spin(int plus_prob) {
		int rand_prob = rand();
		if (rand_prob < plus_prob)
			return +1;
		return -1;
	}

	void magn_beta(unsigned count, double *list_beta, double *list_magn) { //the 3 version
		const int STEP = 500;

		for(unsigned i = 0; i < count; i++) {
			beta = list_beta[i];

			const int NUM = 10;
			double sum = 0;
			for(int i = 0; i < NUM; i++) { //кол-во значений для усреднения
				l->fill_random();
				simulate(STEP);
				double mes = abs(l->avg_magn());
				sum += mes;
				cout << mes << endl;
			}

			list_magn[i] = sum / NUM;
			cout << "---------------" << endl;
			cout << beta << endl;
			cout << "graf_list[" << i << "] = " << list_magn[i] << endl;
			cout << "---------------" << endl;
		}
		/*
		for(unsigned i = 0; i < count; i++) {
			cout << "graf_list[" << i << "] = " << graf_list[i] << endl;
		}
		*/
	}

	void test();

	~Monte_Carlo() {
		delete [] prob_arr;
		delete l;
		cout << "~Monte_Carlo()" << endl;
	}
};

void Monte_Carlo::test() {//test here
	/*l->fill_random();
	cout << "step 0:" << endl;
	l->show();
	cout << "avg. magn = " << l->avg_magn() << endl;

	int steps = 50;
	simulate(steps);
	cout << "step " << steps << ":" << endl;
	l->show();
	cout << "avg. magn = " << l->avg_magn() << endl;*/

	const int S = 10;
	double list_beta[S], list_magn[S];
	for(int i = 0; i < S; i++) {
		list_beta[i] = 0.1 + i * 0.1;
	}
	magn_beta(S, list_beta, list_magn);
}

int main() {
	srand(time(0));

	parameters p(0.5); //steps, beta, H
	square_lattice *l = new square_lattice(64, 64);
	Monte_Carlo model(p, l);
	model.test();
	return 0;
}

int big_rand() { //30-bit random number
	int r = rand() & 0x7FFF;
	r = (r << 15) | (rand() & 0x7FFF);
	return r;
}
