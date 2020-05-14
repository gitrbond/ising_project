#ifndef LATTICE_H
#define LATTICE_H

#include <iostream>
#include <assert.h>
#include <funcs.h>

using namespace std;

class lattice { //abstract
protected:
    int N; //number of spins
    int *L; //spins in array
    int nbrs; //number of nbrs

public:
    lattice(int N, int nbrs);
    lattice(const lattice &old);
    virtual ~lattice();

    lattice& operator = (const lattice &obj);

    int getN() const;
    int* getL();
    int getnbrs() const;
    void fill_random();
    int sum_nbr(int index) const;
    double avg_magn() const;

    virtual void get_nbrs(int index, int *arr) const = 0; //returns array of nbr indexes
    virtual void show() const = 0; //the pure virtual function, prints lattice
};

class square_lattice : public lattice {
    int A, B; //lattice sizes: A strings, B columns

public:
    square_lattice(int A, int B);
    ~square_lattice();

    void get_nbrs(int index, int *arr) const;
    void show() const;
};

#endif // LATTICE_H
