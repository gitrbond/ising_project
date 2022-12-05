#include "hdr/debug.h"
#include <vector>
#include <iostream>

template <typename T>
void Dshow (const char *s, const std::vector <T> &v) {
    std::cout << s << ": {" << std::endl;
	for (auto i = v.begin(); i != v.end(); ++i)
        std::cout << *i << std::endl;
    std::cout << "}" << std::endl;
}

template <typename T>
void Dshow (const char *s, T *arr, int size) {
    std::cout << s << ": {" << std::endl;
	for (int i = 0; i < size; i++)
        std::cout << arr[i] << std::endl;
    std::cout << "}" << std::endl;
}
