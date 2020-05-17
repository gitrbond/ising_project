#include "lattice.h"
#include <iostream>

using namespace std;

lattice::lattice(unsigned int N, unsigned int nbrs) : N(N), L(new int[N]), nbrs(nbrs) {
#ifdef DEBUG
    cout << "lattice(" << N << ")" << endl;
#endif
	try {
		if (N <= 0)
			throw Exception("Invalid lattice size: ", N);
	}
	catch (Exception &exc) {
        cout << exc.what() << exc.Get_data() << endl;
    }
}

lattice::lattice(const lattice &old) : N(old.N), L(new int[N]), nbrs(old.nbrs) {
#ifdef DEBUG
    cout << "lattice(" << N << ") copy constructor" << endl;
#endif
    for (unsigned int i = 0; i < N; i++)
        L[i] = old.L[i];
}

unsigned int lattice::getN() const {
    return N;
}

int* lattice::getL() {
    return L;
}

unsigned int lattice::getnbrs() const {
    return nbrs;
}

void lattice::fill_random() {
    for (unsigned int i = 0; i < N; i++)
        L[i] = 2 * (rand() % 2) - 1;
}

int lattice::sum_nbr(unsigned int index) const { //returns sum of neighbour spins
    unsigned int *nbr_arr = new unsigned int[nbrs];
    get_nbrs(index, nbr_arr);
    int sum = L[nbr_arr[0]];
    for (unsigned int i = 1; i < nbrs; sum += L[nbr_arr[i]], i++);
    delete [] nbr_arr;
    return sum;
}

double lattice::avg_magn() const {//returns average magnetization
    int sum = 0;
    for (unsigned int i = 0; i < N; i++)
        sum += L[i];
    return (double) sum / N;
}

lattice::~lattice() {
#ifdef DEBUG
    cout << "~lattice()" << endl;
#endif
    delete [] L;
}

rect_lattice::rect_lattice(unsigned int A, unsigned int B) : lattice(A * B, 4), A(A), B(B) {
#ifdef DEBUG
    cout << "sq_lattice(" << A << "*" << B << ")" << endl;
#endif
}

void rect_lattice::get_nbrs(unsigned int index, unsigned int *arr) const { //returns array of nbr indexes [U, D, L, R]
    unsigned int a = index / B, b = index % B;
    arr[0] = B * ((a + A - 1) % A) + b;
    arr[1] = B * ((a + 1) % A) + b;
    arr[2] = B * a + (b + B - 1) % B;
    arr[3] = B * a + (b + 1) % B;
}

void rect_lattice::show() const {
    for (unsigned int i = 0; i < A; i++) {
        for (unsigned int j = 0; j < B; j++)
            cout << (L[B * i + j] > 0 ? "+" : ".");
        cout << endl;
    }
}

rect_lattice::~rect_lattice() {
#ifdef DEBUG
    cout << "~sq_lattice()" << endl;
#endif
}

square_lattice::square_lattice(unsigned int A) : rect_lattice(A, A) {
}

linear_lattice::linear_lattice(unsigned int N) : lattice(N, 2) {
}

void linear_lattice::get_nbrs(unsigned int index, unsigned int *arr) const { //returns array of nbr indexes [L, R]
	arr[0] = (index - 1) % N;
	arr[1] = (index + 1) % N;
}

void linear_lattice::show() const {
    for (unsigned int i = 0; i < N; i++)
		cout << (L[i] > 0 ? "+" : ".") << " ";
	cout << endl;
}
