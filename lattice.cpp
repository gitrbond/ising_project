#include "lattice.h"
#include <iostream>

using namespace std;

lattice::lattice(int N, int nbrs) : N(N), L(new int[N]), nbrs(nbrs) {
#ifdef DEBUG
    cout << "lattice(" << N << ")" << endl;
#endif
    assert(nbrs > 0);
}

lattice::lattice(const lattice &old) : N(old.N), L(new int[N]), nbrs(old.nbrs) {
#ifdef DEBUG
    cout << "lattice(" << N << ") copy constructor" << endl;
#endif
    for (int i = 0; i < N; i++)
        L[i] = old.L[i];
}

int lattice::getN() const {
    return N;
}

int* lattice::getL() {
    return L;
}

int lattice::getnbrs() const {
    return nbrs;
}

void lattice::fill_random() {
    for (int i = 0; i < N; i++)
        L[i] = 2 * (rand() % 2) - 1;
}

int lattice::sum_nbr(int index) const { //returns sum of neighbour spins
    assert(nbrs);
    int *nbr_arr = new int[nbrs];
    get_nbrs(index, nbr_arr);
    int sum = L[nbr_arr[0]];
    for (int i = 1; i < nbrs; sum += L[nbr_arr[i]], i++);
    delete [] nbr_arr;
    return sum;
}

double lattice::avg_magn() const {//returns average magnetization
    int sum = 0;
    for (int i = 0; i < N; i++)
        sum += L[i];
    return (double) sum / N;
}

lattice::~lattice() {
#ifdef DEBUG
    cout << "~lattice()" << endl;
#endif
    delete [] L;
}

rect_lattice::rect_lattice(int A, int B) : lattice(A * B, 4), A(A), B(B) {
#ifdef DEBUG
    cout << "sq_lattice(" << A << "*" << B << ")" << endl;
#endif
    assert(A > 0 && B > 0);
}

void rect_lattice::get_nbrs(int index, int *arr) const { //returns array of nbr indexes [U, D, L, R]
    assert(nbrs >= 4);
    int a = index / B, b = index % B;
    arr[0] = B * ((a + A - 1) % A) + b;
    arr[1] = B * ((a + 1) % A) + b;
    arr[2] = B * a + (b + B - 1) % B;
    arr[3] = B * a + (b + 1) % B;
}

void rect_lattice::show() const {
    for (int i = 0; i < A; i++) {
        for (int j = 0; j < B; j++)
            cout << (L[B * i + j] > 0 ? "+" : ".");
        cout << endl;
    }
}

rect_lattice::~rect_lattice() {
#ifdef DEBUG
    cout << "~sq_lattice()" << endl;
#endif
}

square_lattice::square_lattice(int A) : rect_lattice(A, A) {
	assert(A > 0);
}

linear_lattice::linear_lattice(int N) : lattice(N, 2) {
	assert(N > 0);
}

void linear_lattice::get_nbrs(int index, int *arr) const { //returns array of nbr indexes [L, R]
	assert(nbrs >= 2);
	arr[0] = (index - 1) % N;
	arr[1] = (index + 1) % N;
}

void linear_lattice::show() const {
	for (int i = 0; i < N; i++)
		cout << (L[i] > 0 ? "+" : ".") << " ";
	cout << endl;
}
