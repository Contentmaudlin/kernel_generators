#include <cassert>
#include <fib.hpp>

using namespace std;
using namespace kgen;

const string prefix = "prefix"s;

int main() {

    fib_gen g{};

    unsigned long long cur = 1;
    unsigned long long prv = 0;

    for (auto x : g.forall().map([&](auto x) {
                return prefix + to_string(x);
    })) {
        assert(x == prefix + to_string(cur));
        cur += prv;
        prv = cur - prv;

        if (cur > 10000000)
            break;
    }

    return 0;
}
