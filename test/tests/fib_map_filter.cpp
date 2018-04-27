#include <cassert>
#include <vector>
#include <fib.hpp>

using namespace std;
using namespace kgen;

const int iter = 100;

int x5(int x) {
    return x * 5;
}

bool x_even(int x) {
    return x % 2 == 0;
}

int main() {

    fib_gen g1{};
    fib_gen g2{};

    vector<int> v1;
    vector<int> v2;

    int i = 0;
    for (auto x : g1.forall().map(x5).filter(x_even)) {
        v1.push_back(x);
        if (++i > iter)
            break;
    }

    i = 0;
    for (auto x : g2.forall().filter(x_even).map(x5)) {
        v2.push_back(x);

        if (++i > iter)
            break;
    }

    assert(v1.size() == v2.size());

    for (size_t i = 0; i < v1.size(); ++i)
        assert(v1[i] == v2[i]);

    return 0;
}
