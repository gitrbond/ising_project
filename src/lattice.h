#ifndef LATTICE_H
#define LATTICE_H

#include <iostream>
#include <assert.h>

class Exception : public std::exception {
private:
    std::string m_error;
    int data;
public:
    Exception(const std::string& m_error, int data) : m_error(m_error), data(data) {}

    const char* what() const noexcept {
        return m_error.c_str();
    }

    int Get_data() {
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
    unsigned int getnbrs() const;
    void fill_random();
    int sum_nbr(unsigned int index) const;
    double avg_magn() const;

    virtual void get_nbrs(unsigned int index, unsigned int *arr) const = 0; //returns array of nbr indexes
    virtual void show() const = 0; //the pure virtual function, prints lattice
};

class rect_lattice : public lattice {
	const unsigned int A, B; //lattice sizes: A strings, B columns

public:
    rect_lattice(unsigned int A, unsigned int B);
    virtual ~rect_lattice();

	void get_nbrs(unsigned int index, unsigned int *arr) const;
    void show() const;
};

class square_lattice : public rect_lattice {
public:
    square_lattice(unsigned int A);
};

class linear_lattice : public lattice {
public:
    linear_lattice(unsigned int N);

    void get_nbrs(unsigned int index, unsigned int *arr) const;
	void show() const;
};

class tridimensional_lattice : public lattice {
	const unsigned int A, B, C; //lattice sizes

public:
	tridimensional_lattice(unsigned int A, unsigned int B, unsigned int C);
	virtual ~tridimensional_lattice();

	void get_nbrs(unsigned int index, unsigned int *arr) const;
	void show() const;
};

class cubic_lattice : public tridimensional_lattice {
public:
	cubic_lattice(unsigned int A);
};

#endif // LATTICE_H
