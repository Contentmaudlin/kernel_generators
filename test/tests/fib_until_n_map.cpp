#include <cassert>
#include <vector>
#include <fib.hpp>

using namespace std;
using namespace kgen;


string f(int x) {
    return "prefix: "s + to_string(x);
}

const int iters = 100;

int main() {

    fib_gen g{};

    vector<string> xs;
    vector<int> ys;

    unsigned long long cur = 1;
    unsigned long long prv;

    for (auto x : g.until_n(iters).map(f)) {
        xs.push_back(x);
        ys.push_back(cur);

        cur += prv;
        prv = cur - prv;
    }

    assert(xs.size() == ys.size());
    for (size_t i = 0; i < xs.size(); ++i)
        assert(xs[i] == f(ys[i]));

    return 0;
}
