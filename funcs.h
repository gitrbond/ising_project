#ifndef FUNCS_H
#define FUNCS_H

#include <stdlib.h>
#include <vector>

using namespace std;

bool vcontains(const vector <int> &v, int el);
bool vdel(vector <int> &v, int el);
void vlinfill(vector <double> &v, const double start, const double end, const int points);
int big_rand();

#endif // FUNCS_H
