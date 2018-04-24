#include <cassert>
#include "kgen.hpp"

class fib_hist_array : public kgen::gen<int, 2> {
public:
    fib_hist_array() : kgen::gen<int, 2>{{1, 1}} {}    // seed state
private:
    int next() override {
      return hist(-1) + hist(-2);
    }
};


int fib_hist_array_test() {
    using namespace std;

    fib_hist_array g;

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

    cout << "/*** PASSED fib_hist_array ***/" << endl;

    return 0;
}
