#include "hdr/lattice.h"
#include <iostream>

Lattice::Lattice(unsigned int N, unsigned int nbrs) : N(N), L(new int[N]), nbrs(nbrs) {
#ifdef DEBUG
  std::cout << "lattice(" << N << ")" << std::endl;
#endif
  try {
    if (N <= 0)
      throw Exception("Invalid lattice size: ", N);
  }
  catch (Exception &exc) {
    std::cout << exc.what() << exc.getData() << std::endl;
  }
}

Lattice::Lattice(const Lattice &old) : N(old.N), L(new int[N]), nbrs(old.nbrs) {
#ifdef DEBUG
  std::cout << "lattice(" << N << ") copy constructor" << std::endl;
#endif
  for (unsigned int i = 0; i < N; i++)
    L[i] = old.L[i];
}

unsigned int Lattice::getN() const {
  return N;
}

int* Lattice::getL() {
  return L;
}

unsigned int Lattice::getNbrs() const {
  return nbrs;
}

void Lattice::fillRandom() {
  for (unsigned int i = 0; i < N; i++)
    L[i] = 2 * (rand() % 2) - 1;
}

int Lattice::sumNbr(unsigned int index) const { //returns sum of neighbour spins
  unsigned int *nbrArr = new unsigned int[nbrs];
  getNbrs(index, nbrArr);
  int sum = L[nbrArr[0]];
  for (unsigned int i = 1; i < nbrs; sum += L[nbrArr[i]], i++);
  delete [] nbrArr;
  return sum;
}

double Lattice::avgMagn() const {//returns average magnetization
  int sum = 0;
  for (unsigned int i = 0; i < N; i++)
    sum += L[i];
  return (double) sum / N;
}

Lattice::~Lattice() {
#ifdef DEBUG
  std::cout << "~lattice()" << std::endl;
#endif
  delete [] L;
}

RectLattice::RectLattice(unsigned int A, unsigned int B) : Lattice(A * B, 4), A(A), B(B) {
#ifdef DEBUG
  std::cout << "rectLattice(" << A << "*" << B << ")" << std::endl;
#endif
}

void RectLattice::getNbrs(unsigned int index, unsigned int *arr) const { //returns array of nbr indexes [U, D, L, R]
  unsigned int a = index / B, b = index % B;
  arr[0] = B * ((a + A - 1) % A) + b;
  arr[1] = B * ((a + 1) % A) + b;
  arr[2] = B * a + (b + B - 1) % B;
  arr[3] = B * a + (b + 1) % B;
}

void RectLattice::show() const {
  for (unsigned int i = 0; i < A; i++) {
    for (unsigned int j = 0; j < B; j++)
      std::cout << (L[B * i + j] > 0 ? "+" : ".");
    std::cout << std::endl;
  }
}

RectLattice::~RectLattice() {
#ifdef DEBUG
  std::cout << "~rectLattice()" << std::endl;
#endif
}

SquareLattice::SquareLattice(unsigned int A) : RectLattice(A, A) {
}

LinearLattice::LinearLattice(unsigned int N) : Lattice(N, 2) {
}

void LinearLattice::getNbrs(unsigned int index, unsigned int *arr) const { //returns array of nbr indexes [L, R]
  arr[0] = (index - 1) % N;
  arr[1] = (index + 1) % N;
}

void LinearLattice::show() const {
  for (unsigned int i = 0; i < N; i++)
    std::cout << (L[i] > 0 ? "+" : ".") << " ";
  std::cout << std::endl;
}

TridimensionalLattice::TridimensionalLattice(unsigned int A, unsigned int B, unsigned int C) : Lattice(A * B * C, 6), A(A), B(B), C(C) {
#ifdef DEBUG
  std::cout << "tridimensionalLattice(" << A << "*" << B << "*" << C << ")" << std::endl;
#endif
}

void TridimensionalLattice::getNbrs(unsigned int index, unsigned int *arr) const { //returns array of nbr indexes [U, D, L, R, UF, DF] (UF - up floor)
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

void TridimensionalLattice::show() const {
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

TridimensionalLattice::~TridimensionalLattice() {
#ifdef DEBUG
  std::cout << "~tridimensionalLattice()" << std::endl;
#endif
}

CubicLattice::CubicLattice(unsigned int A) : TridimensionalLattice(A, A, A) {
}