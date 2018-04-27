#include <cassert>
#include <fib.hpp>

using namespace std;
using namespace kgen;

int main() {

    fib_gen g{};

    unsigned long long cur = 1;
    unsigned long long prv;

    for (auto x : g.forall()) {
        assert(x == cur);
        cur += prv;
        prv = cur - prv;

        if (cur > 10000000)
            break;
    }

    return 0;
}
