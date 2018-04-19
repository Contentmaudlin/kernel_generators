#include <cassert>
#include "kgen.hpp"

class fib_hist : public kgen::gen<int, 2> {
public:
    fib_hist() : kgen::gen<int, 2>{{}, 1} {}    // seed state
private:
    int next() override {
      return hist(-1) + hist(-2);
    }
};


int fib_hist_test() {
    using namespace std;

    fib_hist g;

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

    cout << "/*** PASSED fib_hist ***/" << endl;

    return 0;
}
