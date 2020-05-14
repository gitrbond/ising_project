#include <iostream>
#include <fstream>
#include <vector> //essential
#include <stdlib.h>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <assert.h>

//#define DEBUG

//#ifdef DEBUG
#include "debug.h" //debug functions
//#endif

using namespace std;

inline bool vcontains(const vector < int > &v, int el);
bool vdel(vector < int > &v, int el);
void vlinfill(vector < double > &v, const double start, const double end, const int points);
int big_rand();

class lattice { //abstract
	lattice& operator = (const lattice &obj); //private assignment

protected:
	const int N; //number of spins
	int *L; //spins in array
	const int nbrs; //number of nbrs

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

	inline int getN() const {
		return N;
	}

	inline int* getL() {
		return L;
	}

	inline int getnbrs() const {
		return nbrs;
	}

	void fill_random() {
		srand(time(NULL));
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

class rect_lattice : public lattice {
	const int A, B; //lattice sizes: A strings, B columns

public:
	rect_lattice(int A, int B) : lattice(A * B, 4), A(A), B(B) {
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

	virtual ~rect_lattice() {
#ifdef DEBUG
		cout << "~rect_lattice()" << endl;
#endif
	}
};

class square_lattice : public rect_lattice {
	const int A; //A x A lattice

public:
	square_lattice(int A) : rect_lattice(A, A), A(A) {
		assert(A > 0);
	}
};

class linear_lattice : public lattice {
public:
	linear_lattice(int N) : lattice(N, 2) {
		assert(N > 0);
	}

	void get_nbrs(int index, int *arr) const { //returns array of nbr indexes [L, R]
		assert(nbrs >= 2);
		arr[0] = (index - 1) % N;
		arr[1] = (index + 1) % N;
	}

	void show() const {
		for (int i = 0; i < N; i++)
			cout << (L[i] > 0 ? "+" : ".") << " ";
		cout << endl;
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
	}

	void set_beta(const double new_beta) {
		beta = new_beta;
	}

	virtual ~parameters() {
	}
};

class Monte_Carlo : public parameters {
public:
	Monte_Carlo (parameters &p) : parameters(p) {
	}

	void simulate(lattice *l, int steps) const {
		int nbrs = l->getnbrs();
		int *prob_arr = new int [1 + l->getnbrs()]; //array of all possible probabilities
		for (int i = 0; i <= nbrs; i++)
			prob_arr[i] =  round(RAND_MAX / (1 + exp(-2 * beta *(2 * i - nbrs) - mu * H)));
		//cout << "mu*H= " << mu*H << endl;
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
			vector <int> Claster {
				spin
			}
			, Pocket {
				spin
			}
			;
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

	void plot_magn_beta(lattice *l, const vector < double > &beta_points, vector < double > &magn_points, const int steps, const int averaging) { //the 5th version
		assert(averaging > 0);
		magn_points.clear();

		const char *filename = "data_magn_beta";
		ofstream output(filename);
		if (output) {
			//output << parametrs;

			for (auto i = beta_points.begin(); i != beta_points.end(); ++i) {
				beta = *i;
				double avg_magn = 0;

				for(int j = 0; j < averaging; j++) {
					l->fill_random();
					simulate(l, steps);
					double mes;
					mes = abs(l->avg_magn());
					output << mes << " ";
					avg_magn += mes;
					cout << ".";
				}
				avg_magn /= averaging;
				magn_points.push_back(avg_magn);
				output << endl << beta << " " << magn_points.back() << endl;
				cout << magn_points.back() << endl;
			}
			output << endl;

			for (auto i = magn_points.begin(); i != magn_points.end(); ++i) {
				output << *i << endl;
			}

		} else
			cerr << "Unable to open output file " << filename << endl;
	}

	void plot_magn_H(lattice *l, const vector < double > &H_points, vector < double > &magn_points, const int plot_beta, const int steps, const int averaging) { //the 5th version
		assert(averaging > 0);
		magn_points.clear();
		beta = plot_beta;

		const char *filename = "data_magn_H";
		ofstream output(filename);
		if (output) {
			//output << parametrs;

			for (auto i = H_points.begin(); i != H_points.end(); ++i) {
				H = *i;
				double avg_magn = 0;

				for(int j = 0; j < averaging; j++) {
					l->fill_random();
					simulate(l, steps);
					double mes;
					mes = l->avg_magn();
					output << mes << " ";
					avg_magn += mes;
					cout << ".";
				}
				avg_magn /= averaging;
				magn_points.push_back(avg_magn);
				output << endl << beta << " " << magn_points.back() << endl;
				cout << magn_points.back() << endl;
			}
			output << endl;

			for (auto i = magn_points.begin(); i != magn_points.end(); ++i) {
				output << *i << endl;
			}

		} else
			cerr << "Unable to open output file " << filename << endl;

	}

	void test(lattice *l);

	~Monte_Carlo() {
#ifdef DEBUG
		cout << "~Monte_Carlo()" << endl;
#endif
	}
};

void Monte_Carlo::test(lattice *l) { //test here
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

	//model.test(l);
	vector <double> x_points, y_points;
	vlinfill(x_points, -0.01, 0.01, 7); //fills vector with 11 values from 0 to 1
	Dshow("H values (x)", x_points);

	model.plot_magn_beta(l, x_points, y_points, 50, 5); // ... steps, averaging
	model.plot_magn_H(l, x_points, y_points, 0.8, 50, 1); // ... beta, steps, averaging
	Dshow("avg_magn values (y)", y_points);

	delete l;
	return 0;
}

inline bool vcontains(const vector < int > &v, int el) { //checks if vector contains element
	return find(v.begin(), v.end(), el) != v.end();
}

bool vdel(vector < int > &v, int el) { //deletes element from vector. returns false if not found
	auto del = find(v.begin(), v.end(), el);
	if (del != v.end()) {
		v.erase(del);
		return true;
	}
	return false;
}

void vlinfill(vector < double > &v, const double start, const double end, const int points) {
	for (int i = 0; i < points; i++)
		v.push_back(start + i * (end - start) / (points - 1));
}

int big_rand() { //30-bit random number
	int r = rand() & 0x7FFF;
	r = (r << 15) | (rand() & 0x7FFF);
	return r;
}
