#include <cassert>
#include <fib.hpp>

using namespace std;
using namespace kgen;

const unsigned long long m = 5;
const int iter = 100;

int main() {

    fib_gen g{};

    int i = 0;
    for (auto x : g.forall().filter([](auto x) { return x % m == 0; } )) {
        assert(x % m == 0);

        if (++i > iter)
            break;
    }

    return 0;
}
