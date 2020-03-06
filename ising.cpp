#include <iostream>
#include <stdio.h>

using namespace std;

class lattice {
protected:
    int N;	//number of spins
    int *L;	//lattice as array

public:
	lattice () : N (0), L (nullptr) {cout << "lattice ()\n";}
	lattice (int N)	: N (N), L (new int [N])	{}
	/*void fill_random ()	{
		//if (
	}*/
	virtual void show ()	{}
	virtual ~lattice ()	{
		delete L;
		cout << "~lattice ()\n";
	}
};

class square_lattice : public lattice	{
public:
	square_lattice ()	{cout << "sq_lattice ()\n";}
	~square_lattice ()	{cout << "~sq_lattice ()\n";}
};

int main ()  {
	lattice *l = new square_lattice ();
	delete l;
    return 0;
}
