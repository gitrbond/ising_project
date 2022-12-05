#ifndef FUNCS_H
#define FUNCS_H

#include <stdlib.h>
#include <vector>

int calc_plot(const char* f_input, const char* f_output, int lsize, int algo, int steps, int averaging = 1);

bool vcontains(const std::vector <int> &v, int el);
bool vdel(std::vector <int> &v, int el);
void vlinfill(std::vector <double> &v, const double start, const double end, const int points);
int rand_30bit();

#endif // FUNCS_H
