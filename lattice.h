#ifndef LATTICE_H
#define LATTICE_H

#include <iostream>
#include <assert.h>
#include <funcs.h>

using namespace std;

class lattice { //abstract
	lattice& operator = (const lattice &obj); //private assignment

protected:
    const unsigned int N; //number of spins
    int *L; //spins in array
    char nbrs; //number of nbrs

public:
    lattice(unsigned int N, char nbrs);
    lattice(const lattice &old);
    virtual ~lattice();

    unsigned int getN() const;
    int* getL();
    char getnbrs() const;
    void fill_random();
    int sum_nbr(int index) const;
    double avg_magn() const;

    virtual void get_nbrs(int index, int *arr) const = 0; //returns array of nbr indexes
    virtual void show() const = 0; //the pure virtual function, prints lattice
};

class rect_lattice : public lattice {
    unsigned int A, B; //lattice sizes: A strings, B columns

public:
    rect_lattice(unsigned int A, unsigned int B);
    virtual ~rect_lattice();

    void get_nbrs(int index, int *arr) const;
    void show() const;
};

class square_lattice : public rect_lattice {
public:
    square_lattice(unsigned int A);
};

class linear_lattice : public lattice {
public:
    linear_lattice(unsigned int N);

	void get_nbrs(int index, int *arr) const;
	void show() const;
};

#endif // LATTICE_H
