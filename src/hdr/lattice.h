#ifndef LATTICE_H
#define LATTICE_H

#include <iostream>
#include <assert.h>

class exception : public std::exception {
private:
    std::string m_error;
    int data;
public:
    exception(const std::string& m_error, int data) : m_error(m_error), data(data) {}

    const char* what() const noexcept {
        return m_error.c_str();
    }

    int getData() {
        return data;
    }
};

class lattice { //abstract
	lattice& operator = (const lattice &obj); //private assignment

protected:
    const unsigned int N; //number of spins
    int *L; //spins in array
    const unsigned int nbrs; //number of spin neighbors

public:
    lattice(unsigned int N, unsigned int nbrs);
    lattice(const lattice &old);
    virtual ~lattice();

    unsigned int getN() const;
    int* getL();
    unsigned int getNbrs() const;
    void fillRandom();
    int sumNbr(unsigned int index) const;
    double avgMagn() const;

    virtual void getNbrs(unsigned int index, unsigned int *arr) const = 0; //returns array of nbr indexes
    virtual void show() const = 0; //the pure virtual function, prints lattice
};

class rectLattice : public lattice {
	const unsigned int A, B; //lattice sizes: A strings, B columns

public:
    rectLattice(unsigned int A, unsigned int B);
    virtual ~rectLattice();

	void getNbrs(unsigned int index, unsigned int *arr) const;
    void show() const;
};

class squareLattice : public rectLattice {
public:
    squareLattice(unsigned int A);
};

class linearLattice : public lattice {
public:
    linearLattice(unsigned int N);

    void getNbrs(unsigned int index, unsigned int *arr) const;
	void show() const;
};

class tridimensionalLattice : public lattice {
	const unsigned int A, B, C; //lattice sizes

public:
	tridimensionalLattice(unsigned int A, unsigned int B, unsigned int C);
	virtual ~tridimensionalLattice();

	void getNbrs(unsigned int index, unsigned int *arr) const;
	void show() const;
};

class cubicLattice : public tridimensionalLattice {
public:
	cubicLattice(unsigned int A);
};

#endif // LATTICE_H
