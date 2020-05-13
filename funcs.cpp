#include <funcs.h>
#include <algorithm>

bool vcontains(const vector <int> &v, int el) { //checks if vector contains element
    return find(v.begin(), v.end(), el) != v.end();
}

bool vdel(vector <int> &v, int el) { //deletes element from vector. returns false if not found
    auto del = find(v.begin(), v.end(), el);
    if (del != v.end()) {
        v.erase(del);
        return true;
    }
    return false;
}

int big_rand() { //30-bit random number
    int r = rand() & 0x7FFF;
    r = (r << 15) | (rand() & 0x7FFF);
    return r;
}
