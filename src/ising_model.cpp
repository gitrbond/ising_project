#include "hdr/ising_model.h"
#include "hdr/funcs.h"
#include <assert.h>
#include <math.h>
#include <fstream>

Parameters::Parameters(double beta, double H, double J, double mu) :
beta(beta), H(H), J(J), mu(mu) {
}

void Parameters::setBeta(const double newBeta) {
  beta = newBeta;
}

Parameters::~Parameters() {
}

MonteCarlo::MonteCarlo(Parameters &p) : Parameters(p) {
}

void MonteCarlo::heatBathSimulate(Lattice *l, int steps) const {
  int randSpin, prob, nbrs = l->getNbrs(), *L = l->getL(), N = l->getN();
  int probArr[1 + 2 * nbrs]; // Array of all possible probabilities
  for (int i = -nbrs; i <= nbrs; ++i) // Filling the array
    probArr[i + nbrs] = RAND_MAX / (1 + exp(-2 * beta * i - mu * H));
  for (int i = 0; i < steps; ++i) {
    for (int j = 0; j < N; ++j) {
      randSpin = randInRange(0, N); // Choose an arbitrary spin
      prob = probArr[l->sumNbr(randSpin) + nbrs]; // Take the previously calculated probability from the array
      L[randSpin] = defSpin(prob); // Assign +1 or -1
    }
  }
}

void MonteCarlo::clustersSimulate(Lattice *l, int steps) const {
  int spin, *L = l->getL(), N = l->getN();
  unsigned int nbrs = l->getNbrs(), nbrArr[nbrs];
  int prob = RAND_MAX * (1 - exp(-2 * beta)); // The magic number
  for (int j = 0; j < steps; ++j) {
    spin = randInRange(0, N); 						// Arbitrary choice of spin
    std::vector <int> Cluster {spin}, Pocket {spin}; // Put the spin in the cluster and in the pocket
    while (!Pocket.empty()) {
      spin = Pocket[randInRange(0, Pocket.size())]; 	// An arbitrary choice from the pocket
      l->getNbrs(spin, nbrArr); 				// Get neighbors spin
      for (unsigned int i = 0; i < nbrs; ++i) { 			// Check all the neighbors:
        if (L[spin] == L[nbrArr[i]] && 			// If the neighbor's spin is the same
        !vContains(Cluster, nbrArr[i]) && 		// and it's not in the cluster yet,
        rand() < prob) { 							// then with probability prob
          Pocket.push_back(nbrArr[i]); 				// Add to Pocket
          Cluster.push_back(nbrArr[i]);				// Add to Cluster
        }
      }
      vDel(Pocket, spin); 						// Remove from pocket
    }
    for (auto i = Cluster.begin(); i != Cluster.end(); ++i)
      L[*i] = - L[*i]; 						// Flip the whole cluster
      if (Cluster.size() > 5 * l->getN() / 6) // Cuts unnecessary calculations
        break;
  }
}

int MonteCarlo::defSpin(int plusProb) const {
  int randProb = rand();
  if (randProb < plusProb)
    return 1;
  return -1;
}

void MonteCarlo::plotMagnBeta(Lattice *l, const std::vector <double> &betaPoints, std::vector <double> &magnPoints, const int steps, const int averaging, const int algo) {
  try {
    if (averaging <= 0)
      throw Exception("averaging must be positive, you entered: ", averaging);

    magnPoints.clear();
    std::cout << "Plot of the function average magnetization from temperature beta, avg_magn(beta)" << std::endl;
    if (algo == 0)
      std::cout << "Heat bath algorithm" << std::endl;
    if (algo == 1)
      std::cout << "Clasters algorithm" << std::endl;
    for (auto i = betaPoints.begin(); i != betaPoints.end(); ++i) {
      beta = *i;
      double avgMagn = 0;
      double mes;
      for(int j = 0; j < averaging; j++) {
        l->fillRandom();
        if (algo == 0)
          heatBathSimulate(l, steps);
        if (algo == 1)
          clustersSimulate(l, steps);
        mes = l->avgMagn();
        avgMagn += abs(mes);
      }
      avgMagn /= averaging;
      magnPoints.push_back(avgMagn);
      std::cout << "beta = " << beta << "\tavg_magn = " << magnPoints.back() << std::endl;
    }
  }
  catch (Exception &exc) {
    std::cout << exc.what() << exc.getData() << std::endl;
  }
}

void MonteCarlo::test(Lattice *l) {//test here
  l->fillRandom();
  std::cout << "step 0:" << std::endl;
  l->show();
  std::cout << "avg. magn = " << l->avgMagn() << std::endl;

  int steps = 300;
  heatBathSimulate(l, steps);
  //clusters_simulate(l);
  std::cout << "step " << steps << ":" << std::endl;
  l->show();
  std::cout << "avg. magn = " << l->avgMagn() << std::endl;
}

int simulationExample() {
  srand((unsigned)time(NULL));
  Parameters p(0.55); //beta
  SquareLattice *l = new SquareLattice(10);
  MonteCarlo model(p);
  model.test(l);
  delete l;
  return 0;
}