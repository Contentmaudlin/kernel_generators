#include <cassert>
#include <fib.hpp>

using namespace std;
using namespace kgen;

string f(int x) {
    return "prefix"s + to_string(x);
}

int main() {

    fib_gen g{};

    unsigned long long cur = 1;
    unsigned long long prv;

    for (auto x : g.forall().map(f)) {
        assert(x == f(cur));
        cur += prv;
        prv = cur - prv;

        if (cur > 10000000)
            break;
    }

    return 0;
}
