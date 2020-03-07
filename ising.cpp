#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

class lattice {
protected:
	int N;	//number of spins
	int *L;	//lattice stores as array

public:
	lattice () : N (0), L (nullptr)	{
		cout << "lattice ()" << endl;
	}

	lattice (int N)	: N (N), L (new int [N]) {
		cout << "lattice (" << N << ")" << endl;
	}

	void fill_random() {
		srand(time(0));
		for (int i = 0; i < N; i++)
			L[i] = 2 * (rand() % 2) - 1;
	}

	virtual void show() {}

	virtual ~lattice() {
		delete L;
		cout << "~lattice ()" << endl;
	}
};

class square_lattice : public lattice {
	int A, B;
public:
	square_lattice() : A (0), B (0)	{
		cout << "sq_lattice ()" << endl;
	}
	square_lattice (int A, int B) : lattice (A * B), A (A), B (B) {
		cout << "sq_lattice (A*B)" << endl;
	}
	void show() {
		for (int i = 0; i < N; i++) {
			printf("% *d", 3, L[i]);
			if (i % A == A - 1)
				cout << '\n';
		}
	}
	~square_lattice() {
		cout << "~sq_lattice ()" << endl;
	}
};

int main () {
	lattice *l = new square_lattice(4, 3);
	l->fill_random();
	l->show();
	delete l;
	return 0;
}
