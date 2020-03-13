#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctime>

using namespace std;

class lattice {
protected:
	int N; //number of spins
	int *L; //spins in array

public:
	lattice(int N) : N(N), L(new int[N]) {
		cout << "lattice(" << N << ")" << endl;
	}

	void fill_random() {
		srand(time(0));
		for (int i = 0; i < N; i++)
			L[i] = 2 * (rand() % 2) - 1;
	}

	virtual int sum_neighbours(int index) {//returns sum of neighbour spins
	}

	virtual void show() {
	}

	virtual ~lattice() {
		delete L;
		cout << "~lattice()" << endl;
	}

	friend class Monte_Carlo;
};

class square_lattice : public lattice {
	int A, B; //lattice sizes: A strings, B columns

public:
	square_lattice(int A, int B) : lattice(A * B), A(A), B(B) {
		cout << "sq_lattice(" << A << "*" << B << ")" << endl;
	}

	int sum_neighbours(int index) {
		int a = index / B, b = index % B;
		return L[B * ((a + A - 1) % A) + b] +
		+ L[B * ((a + 1) % A) + b] +
		+ L[B * a + (b + 1) % B] +
		+ L[B * a + (b + B - 1) % B];
	}

	void show() {
		for (int i = 0; i < A; i++) {
			for (int j = 0; j < B; j++)
				printf("% *d", 3, L[B * i + j]);
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
	double T; //temperature in Kelvins
	double beta; //beta = 1/kT
	double J; //constant of material
	double h; //outer magnet field

public:
	parameters(int steps, double beta, double h = 0, double J = 1) : steps(steps), beta(beta), J(J), h(h) {
	}
};

class Monte_Carlo : public parameters { //parameters is parent for M-Cמûנאהכמûא
    lattice *l;

public:
	Monte_Carlo (parameters p, lattice *l) : parameters(p), l(l) {
		cout << "Monte_Carlo()" << endl;
	}

	void simulate()	{
		//code me!
	}

	void test();

	~Monte_Carlo() {
		delete l;
		cout << "~Monte_Carlo()" << endl;
	}
};

void Monte_Carlo::test() {//debug here
	l->fill_random();
	l->show();
	for (int i = 0; i < l->N; i++)
		printf("%d ", l->sum_neighbours(i));
	cout << endl;
	//cout << "steps = " << steps << endl;
	//cout << "N = " << l->N << endl;
}

int main() {
	parameters p(100, 0.5); //steps, beta
	Monte_Carlo model(p, new square_lattice(4, 3));
	model.test();
	return 0;
}
