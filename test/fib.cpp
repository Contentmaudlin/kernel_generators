#include <cassert>
#include "kgen.hpp"

class fib_gen : public kgen::gen<int> {
public:
    fib_gen() : kgen::gen<int>{{x}} {}    // seed state
private:
    int next() override {
        x = x[-1] + x[-2];
        return *(*x == 0 ? x = 1 : x);
    }

    lookback<int, 2> x;
};


int fib_test() {
    using namespace std;

    fib_gen g;

    cout << *g  << endl;
    cout << *++g  << endl;
    cout << *++g  << endl;
    cout << *++g << endl;
    cout << *++g  << endl;
    cout << *++g  << endl;
    cout << *++g  << endl;
    cout << *++g  << endl;
    cout << *++g  << endl;
    cout << *++g  << endl;
    cout << *++g  << endl;
    cout << *++g  << endl;
    cout << *++g  << endl;

    cout << "/*** PASSED fib_test ***/" << endl;

    return 0;
}
