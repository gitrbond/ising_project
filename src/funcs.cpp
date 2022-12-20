#include "hdr/funcs.h"
#include "hdr/lattice.h"
#include "hdr/ising_model.h"
#include <algorithm>
#include <fstream>
#include <ctime>
#include <iostream>

bool vContains(const std::vector <int> &v, int el) { //checks if vector contains element
  return find(v.begin(), v.end(), el) != v.end();
}

bool vDel(std::vector <int> &v, int el) { //deletes element from vector. returns false if not found
  auto del = find(v.begin(), v.end(), el);
  if (del != v.end()) {
    v.erase(del);
    return true;
  }
  return false;
}

void vLinFill(std::vector <double> &v, const double start, const double end, const int points) {
  for (int i = 0; i < points; i++)
    v.push_back(start + i * (end - start) / (points - 1));
}

int rand30Bit() { //30-bit random number
  int r = rand() & 0x7FFF;
  r = (r << 15) | (rand() & 0x7FFF);
  return r;
}

int rand25Bit() { //25-bit random number
  int r = rand() & 0x1FFF;
  r = (r << 12) | (rand() & 0x1FFF);
  return r;
}

int randInRange(int start, int end) {
  return start + rand28Bit() % (end - start);
}

int calcPlot(const char* fInput, const char* fOutput, int lSize, int algo, int steps, int averaging) {
  ifstream input(fInput);
  if (input) {
    ofstream output(fOutput);
    if (output) {
      srand((unsigned)time(NULL));
      unsigned n;
      input >> n;
      if (n <= 0) {
        std::cout << "wrong input file format: number of dots n > 0 should go first, then n values" << std::endl;
        return 3;
      }
      //std::cout << "input file: " << f_input << std::endl;
      //std::cout << "output file: " << f_output << std::endl;
      std::vector <double> betaPoints(n), magnPoints;
      for (unsigned i = 0; i < n; i++)
        input >> betaPoints[i];

      Parameters p;
      SquareLattice *l = new SquareLattice(lSize);
      MonteCarlo model(p);
      model.plotMagnBeta(l, betaPoints, magnPoints, steps, averaging, algo);

      delete l;
      for (unsigned i = 0; i < n; i++)
        output << magnPoints[i] << std::endl;
    }
    else {
      std::cout << "Cannot open " << fOutput << " as output" << std::endl;
      return 2;
    }
  }
  else {
    std::cout << "Cannot open " << fInput << " as input" << std::endl;
    return 1;
  }
  return 0;
}
