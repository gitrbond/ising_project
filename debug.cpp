#include "debug.h"
#include <vector>
#include <iostream>

using namespace std;

template <typename T>
void Dshow (const char *s, const vector <T> &v) {
	cout << s << ": {" << endl;
	for (auto i = v.begin(); i != v.end(); ++i)
		cout << *i << endl;
	cout << "}" << endl;
}

template <typename T>
void Dshow (const char *s, T *arr, int size) {
	cout << s << ": {" << endl;
	for (int i = 0; i < size; i++)
		cout << arr[i] << endl;
	cout << "}" << endl;
}
