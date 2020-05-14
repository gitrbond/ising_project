#include <debug.h>
#include <vector>
#include <iostream>

using namespace std;

void Dshow (const char *s, const vector <int> &v) {
    cout << s << ": {";
    for (auto i = v.begin(); i != v.end(); ++i)
        cout << *i << " ";
    cout << "}" << endl;
}

void Dshow (const char *s, int *arr, int size) {
    cout << s << ": {";
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << "}" << endl;
}
