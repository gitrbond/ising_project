#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctime>

using namespace std;

int big_rand();

class lattice {
protected:
	int N; //number of spins
	int *L; //spins in array
	int nbrs; //nuber of nbrs

public:
	lattice(int N, int nbrs) : N(N), L(new int[N]), nbrs(nbrs) {
		cout << "lattice(" << N << ")" << endl;
	}

	void fill_random() {
		srand(time(0));
		for (int i = 0; i < N; i++)
			L[i] = 2 * (rand() % 2) - 1;
	}

	virtual int sum_nbr(int index) {//returns sum of neighbour spins
		return index = 0;
	}

	virtual void show() {
	}

	virtual ~lattice() {
		delete [] L;
		cout << "~lattice()" << endl;
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
	int steps; //number of steps in simulation
	//double T; //temperature in Kelvins - needs later
	double beta; //beta = 1/kT
	double J; //constant of material
	double h; //outer magnet field

public:
	parameters(int steps, double beta, double h = 0, double J = 1) : steps(steps), beta(beta), J(J), h(h) {
	}
};

class Monte_Carlo : public parameters {
	lattice *l;
	double *prob_arr; //array of all possible probabilities

public:
	Monte_Carlo (parameters p, lattice *l) : parameters(p), l(l), prob_arr(new double [1 + l->nbrs]) {
		for (int i = 0; i <= l->nbrs; i++)
			prob_arr[i] =  1 / (1 + exp(-2 * (2 * i - l->nbrs) * beta));
		cout << "Monte_Carlo()" << endl;
	}

	void simulate()	{
		for (int i = 0; i < steps; i++) {
			for (int j = 0; j < l->N; j++) {
				int rand_spin = big_rand() % l->N;
				if (((double) rand() / RAND_MAX) < prob_arr[(l->nbrs + l->sum_nbr(rand_spin)) / 2])
					l->L[rand_spin] = 1;
				else
					l->L[rand_spin] = -1;
			}
		}
	}

	void test();

	~Monte_Carlo() {
		delete [] prob_arr;
		delete l;
		cout << "~Monte_Carlo()" << endl;
	}
};

void Monte_Carlo::test() {//test here
	l->fill_random();
	cout << "step 0:" << endl;
	l->show();
	cout << "step " << steps << ":" << endl;
	simulate();
	l->show();
}

int main() {
	parameters p(500, 0.44); //steps, beta
	Monte_Carlo model(p, new square_lattice(64, 64));
	model.test();
	return 0;
}

int big_rand() { //30-bit random number
	int r = rand() & 0x7FFF;
	r = (r << 15) | (rand() & 0x7FFF);
	return r;
}
