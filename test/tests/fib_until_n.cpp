#include <cassert>
#include <fib.hpp>

using namespace std;
using namespace kgen;


const int iters = 100;

int main() {

    fib_gen g{};

    unsigned long long cur = 1;
    unsigned long long prv = 0;
    int ctr = 0;

    for (auto x : g.until_n(iters)) {
        assert(x == cur);
        cur += prv;
        prv = cur - prv;
        ++ctr;
    }

    assert(ctr == iters);

    return 0;
}
