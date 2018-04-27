#include <cassert>
#include <fib.hpp>

using namespace std;
using namespace kgen;

const unsigned long long limit = 10000000;

int main() {

    fib_gen g{};

    unsigned long long cur = 1;
    unsigned long long prv = 0;

    for (auto x : g.until([](auto x) { return x > limit; })) {
        assert(x == cur);
        cur += prv;
        prv = cur - prv;
    }

    assert(prv <= limit); 

    return 0;
}
