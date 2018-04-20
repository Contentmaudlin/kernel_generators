#include <cassert>
#include "kgen.hpp"

class fibnobase_gen : public kgen::gen<int> {
public:
    fibnobase_gen() : kgen::gen<int>{1, {x}} {}  // seed state
private:
    int next() override {
        x = x[-1] + x[-2];
        return *x;
    }

    lookback<int, 2> x{1}; // x will have a lookback history of 2 iterations
};

int fibnobase_test() {
    using namespace std;

    fibnobase_gen g;


    
    cout << *g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;

    cout << "/*** PASSED fibnobase_test() ****/" << endl;

    return 0;
}
