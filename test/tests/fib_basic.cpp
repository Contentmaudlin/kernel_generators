#include <cassert>
#include <fib.hpp>

using namespace std;
using namespace kgen;

int main() {
    fib_gen g{};

    unsigned long long cur = 1;
    unsigned long long prv = 0;

    for (int i = 0; i < 100; ++i) {
        assert(*g == cur);
        ++g;
        
        cur += prv;
        prv = cur - prv;
    }

    return 0;
}
