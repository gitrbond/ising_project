#include "hdr/lattice.h"
#include <iostream>

lattice::lattice(unsigned int N, unsigned int nbrs) : N(N), L(new int[N]), nbrs(nbrs) {
#ifdef DEBUG
    std::cout << "lattice(" << N << ")" << std::endl;
#endif
	try {
		if (N <= 0)
			throw exception("Invalid lattice size: ", N);
	}
	catch (exception &exc) {
        std::cout << exc.what() << exc.getData() << std::endl;
	}
}

lattice::lattice(const lattice &old) : N(old.N), L(new int[N]), nbrs(old.nbrs) {
#ifdef DEBUG
    std::cout << "lattice(" << N << ") copy constructor" << std::endl;
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

unsigned int lattice::getNbrs() const {
	return nbrs;
}

void lattice::fillRandom() {
	for (unsigned int i = 0; i < N; i++)
		L[i] = 2 * (rand() % 2) - 1;
}

int lattice::sumNbr(unsigned int index) const { //returns sum of neighbour spins
	unsigned int *nbr_arr = new unsigned int[nbrs];
	getNbrs(index, nbr_arr);
	int sum = L[nbr_arr[0]];
	for (unsigned int i = 1; i < nbrs; sum += L[nbr_arr[i]], i++);
	delete [] nbr_arr;
	return sum;
}

double lattice::avgMagn() const {//returns average magnetization
	int sum = 0;
	for (unsigned int i = 0; i < N; i++)
		sum += L[i];
	return (double) sum / N;
}

lattice::~lattice() {
#ifdef DEBUG
    std::cout << "~lattice()" << std::endl;
#endif
	delete [] L;
}

rectLattice::rectLattice(unsigned int A, unsigned int B) : lattice(A * B, 4), A(A), B(B) {
#ifdef DEBUG
    std::cout << "rectLattice(" << A << "*" << B << ")" << std::endl;
#endif
}

void rectLattice::getNbrs(unsigned int index, unsigned int *arr) const { //returns array of nbr indexes [U, D, L, R]
	unsigned int a = index / B, b = index % B;
	arr[0] = B * ((a + A - 1) % A) + b;
	arr[1] = B * ((a + 1) % A) + b;
	arr[2] = B * a + (b + B - 1) % B;
	arr[3] = B * a + (b + 1) % B;
}

void rectLattice::show() const {
	for (unsigned int i = 0; i < A; i++) {
		for (unsigned int j = 0; j < B; j++)
            std::cout << (L[B * i + j] > 0 ? "+" : ".");
        std::cout << std::endl;
	}
}

rectLattice::~rectLattice() {
#ifdef DEBUG
    std::cout << "~rectLattice()" << std::endl;
#endif
}

squareLattice::squareLattice(unsigned int A) : rectLattice(A, A) {
}

linearLattice::linearLattice(unsigned int N) : lattice(N, 2) {
}

void linearLattice::getNbrs(unsigned int index, unsigned int *arr) const { //returns array of nbr indexes [L, R]
	arr[0] = (index - 1) % N;
	arr[1] = (index + 1) % N;
}

void linearLattice::show() const {
	for (unsigned int i = 0; i < N; i++)
        std::cout << (L[i] > 0 ? "+" : ".") << " ";
    std::cout << std::endl;
}

tridimensionalLattice::tridimensionalLattice(unsigned int A, unsigned int B, unsigned int C) : lattice(A * B * C, 6), A(A), B(B), C(C) {
#ifdef DEBUG
    std::cout << "tridimensionalLattice(" << A << "*" << B << "*" << C << ")" << std::endl;
#endif
}

void tridimensionalLattice::getNbrs(unsigned int index, unsigned int *arr) const { //returns array of nbr indexes [U, D, L, R, UF, DF] (UF - up floor)
	int AB = A * B;
	int floor = index / (AB), new_index = index % (AB);
	int a = new_index / B, b = new_index % B;
	int S = floor * (AB);
	arr[0] = B * ((a + A - 1) % A) + b + S;
	arr[1] = B * ((a + 1) % A) + b + S;
	arr[2] = B * a + (b + B - 1) % B + S;
	arr[3] = B * a + (b + 1) % B + S;
	arr[4] = (index + AB) % N;
	arr[5] = (index - AB + N) % N;
}

void tridimensionalLattice::show() const {
	unsigned int spin = 0, i, j, k;
	for (i = 0; i < C; i++) {
        std::cout << i << " floor:" << std::endl;
		for (j = 0; j < A; j++) {
			for (k = 0; k < B; k++) {
                std::cout << (L[spin] > 0 ? "+" : ".");
				spin++;
			}
            std::cout << std::endl;
		}
        std::cout << std::endl;
	}
}

tridimensionalLattice::~tridimensionalLattice() {
#ifdef DEBUG
    std::cout << "~tridimensionalLattice()" << std::endl;
#endif
}

cubicLattice::cubicLattice(unsigned int A) : tridimensionalLattice(A, A, A) {
}
