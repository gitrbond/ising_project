#ifndef DEBUG_H
#define DEBUG_H

#include <vector>

template <typename T>
void dShow (const char *s, const std::vector <T> &v);

template <typename T>
void dShow (const char *s, T *arr, int size);


#endif // DEBUG_H
