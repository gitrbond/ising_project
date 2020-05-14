#ifndef DEBUG_H
#define DEBUG_H

#include <vector>

using namespace std;

template <typename T>
void Dshow (const char *s, const vector <T> &v);

template <typename T>
void Dshow (const char *s, T *arr, int size);

#endif // DEBUG_H
