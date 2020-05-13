#include <iostream>
#include <vector> //essential
#include <stdlib.h>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <assert.h>

//#define DEBUG

#ifdef DEBUG
#include "debug.h" //debug functions
#endif

using namespace std;

inline bool vcontains(const vector <int> &v, int el);
bool vdel(vector <int> &v, int el);
int big_rand();

class lattice { //abstract
protected:
	int N; //number of spins
	int *L; //spins in array
	int nbrs; //number of nbrs

public:
	lattice(int N, int nbrs) : N(N), L(new int[N]), nbrs(nbrs) {
#ifdef DEBUG
		cout << "lattice(" << N << ")" << endl;
#endif
		assert(nbrs > 0);
	}

	lattice(const lattice &old) : N(old.N), L(new int[N]), nbrs(old.nbrs) {
#ifdef DEBUG
		cout << "lattice(" << N << ") copy constructor" << endl;
#endif
		for (int i = 0; i < N; i++)
			L[i] = old.L[i];
	}

	lattice& operator = (const lattice &obj) {
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

	int getN() {
		return N;
	}

	int* getL() {
		return L;
	}

	int getnbrs() {
		return nbrs;
	}

	void fill_random() {
		for (int i = 0; i < N; i++)
			L[i] = 2 * (rand() % 2) - 1;
	}

	int sum_nbr(int index) const { //returns sum of neighbour spins
		assert(nbrs);
		int *nbr_arr = new int[nbrs];
		get_nbrs(index, nbr_arr);
		int sum = L[nbr_arr[0]];
		for (int i = 1; i < nbrs; sum += L[nbr_arr[i]], i++);
		delete [] nbr_arr;
		return sum;
	}

	virtual void get_nbrs(int index, int *arr) const = 0; //returns array of nbr indexes

	virtual void show() const = 0; //the pure virtual function, prints lattice

	double avg_magn() const {//returns average magnetization
		int sum = 0;
		for (int i = 0; i < N; i++)
			sum += L[i];
		return (double) sum / N;
	}

	virtual ~lattice() {
#ifdef DEBUG
		cout << "~lattice()" << endl;
#endif
		delete [] L;
	}
};

class square_lattice : public lattice {
	int A, B; //lattice sizes: A strings, B columns

public:
	square_lattice(int A, int B) : lattice(A * B, 4), A(A), B(B) {
#ifdef DEBUG
		cout << "sq_lattice(" << A << "*" << B << ")" << endl;
#endif
		assert(A > 0 && B > 0);
	}

	void get_nbrs(int index, int *arr) const { //returns array of nbr indexes [U, D, L, R]
		assert(nbrs >= 4);
		int a = index / B, b = index % B;
		arr[0] = B * ((a + A - 1) % A) + b;
		arr[1] = B * ((a + 1) % A) + b;
		arr[2] = B * a + (b + B - 1) % B;
		arr[3] = B * a + (b + 1) % B;
	}

	void show() const {
		for (int i = 0; i < A; i++) {
			for (int j = 0; j < B; j++)
				cout << (L[B * i + j] > 0 ? "+" : ".");
			cout << endl;
		}
	}

	~square_lattice() {
#ifdef DEBUG
		cout << "~sq_lattice()" << endl;
#endif
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
#ifdef DEBUG
		cout << "parameters()" << endl;
#endif
	}

	void set_beta(const double new_beta) {
		beta = new_beta;
	}

	virtual ~parameters() {
#ifdef DEBUG
		cout << "~parameters()" << endl;
#endif
	}
};

class Monte_Carlo : public parameters {
public:
	Monte_Carlo (parameters &p) : parameters(p) {
#ifdef DEBUG
		cout << "Monte_Carlo()" << endl;
#endif
	}

	void simulate(lattice *l, int steps) const {
		int nbrs = l->getnbrs();
		int *prob_arr = new int [1 + l->getnbrs()]; //array of all possible probabilities
		for (int i = 0; i <= nbrs; i++)
			prob_arr[i] =  round(RAND_MAX / (1 + exp(-2 * beta *((2 * i - nbrs) + mu * H))));
		for (int i = 0; i < steps; i++) {
			for (int j = 0; j < l->getN(); j++) {
				int rand_spin = big_rand() % l->getN();
				assert((nbrs + l->sum_nbr(rand_spin)) / 2 < 1 + nbrs);
				int prob = prob_arr[(nbrs + l->sum_nbr(rand_spin)) / 2];
				l->getL()[rand_spin] = def_spin(prob);
			}
		}
		delete [] prob_arr;
	}

	void clasters_simulate(lattice *l) const {
		int steps = 10 * sqrt(l->getN()) * exp(-3 * beta);
		int prob = RAND_MAX * (1 - exp(-2 * beta)); //magical number
		int *nbr_arr = new int[l->getnbrs()];
		for (int j = 0; j < steps; j++) {
			int spin = big_rand() % l->getN();
			vector <int> Claster {spin}, Pocket {spin};
			while (Pocket.size()) {
				spin = Pocket[big_rand() % Pocket.size()]; //randomly choose from pocket
				l->get_nbrs(spin, nbr_arr);
				for (int i = 0; i < l->getnbrs(); i++) {
					if (rand() < prob && //take spin with probability
						l->getL()[spin] == l->getL()[nbr_arr[i]] && !vcontains(Claster, nbr_arr[i])) {
						Pocket.push_back(nbr_arr[i]); //add it to pocket
						Claster.push_back(nbr_arr[i]); //and to claster
					}
				}
				assert(vdel(Pocket, spin)); //delete from pocket
#ifdef DEBUG
				Dshow("Pocket", Pocket);
				Dshow("Claster", Claster);
#endif
			}
			for (auto i = Claster.begin(), end = Claster.end(); i != end; ++i)
				l->getL()[*i] = -l->getL()[*i]; //flipping claster
		}
		delete [] nbr_arr;
	}

	int def_spin(int plus_prob) const {
		int rand_prob = rand();
		if (rand_prob < plus_prob)
			return 1;
		return -1;
	}

	void test(lattice *l);
};

void Monte_Carlo::test(lattice *l) {//test here
	l->fill_random();
	cout << "step 0:" << endl;
	l->show();
	cout << "avg. magn = " << l->avg_magn() << endl;

	int steps = 300;
	simulate(l, 1);
	//clasters_simulate(l);
	cout << "step " << steps << ":" << endl;
	l->show();
	cout << "avg. magn = " << l->avg_magn() << endl;
}

int main() {
	srand((unsigned)time(NULL));
	parameters p(0.55); //beta
	square_lattice *l = new square_lattice(5, 5);
	Monte_Carlo model(p);
	model.test(l);
	delete l;
	return 0;
}

inline bool vcontains(const vector <int> &v, int el) { //checks if vector contains element
    return find(v.begin(), v.end(), el) != v.end();
}

bool vdel(vector <int> &v, int el) { //deletes element from vector. returns false if not found
	auto del = find(v.begin(), v.end(), el);
	if (del != v.end()) {
		v.erase(del);
		return true;
	}
	return false;
}

int big_rand() { //30-bit random number
	int r = rand() & 0x7FFF;
	r = (r << 15) | (rand() & 0x7FFF);
	return r;
}
