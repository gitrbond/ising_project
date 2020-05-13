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

lattice& lattice::operator = (const lattice &obj) {
#ifdef DEBUG
    cout << "opertator = (const lattice &obj) assignment" << endl;
#endif
    N = obj.N;
    L = new int[N];
    nbrs = obj.nbrs;
    for (int i = 0; i < N; i++)
        L[i] = obj.L[i];
    return *this;
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

square_lattice::square_lattice(int A, int B) : lattice(A * B, 4), A(A), B(B) {
#ifdef DEBUG
    cout << "sq_lattice(" << A << "*" << B << ")" << endl;
#endif
    assert(A > 0 && B > 0);
}

void square_lattice::get_nbrs(int index, int *arr) const { //returns array of nbr indexes [U, D, L, R]
    assert(nbrs >= 4);
    int a = index / B, b = index % B;
    arr[0] = B * ((a + A - 1) % A) + b;
    arr[1] = B * ((a + 1) % A) + b;
    arr[2] = B * a + (b + B - 1) % B;
    arr[3] = B * a + (b + 1) % B;
}

void square_lattice::show() const {
    for (int i = 0; i < A; i++) {
        for (int j = 0; j < B; j++)
            cout << (L[B * i + j] > 0 ? "+" : ".");
        cout << endl;
    }
}

square_lattice::~square_lattice() {
#ifdef DEBUG
    cout << "~sq_lattice()" << endl;
#endif
}
