#include <cassert>
#include "kgen.hpp"

class fibnobase_gen : public kgen::gen<int, 2> {
public:
    fibnobase_gen() : kgen::gen<int, 2>{{x}} {}    // seed state
private:
    int next() override {
        x = x[-1] + x[-2];
        return *x;
    }

    lookback<int, 2> x{1}; // x will have a lb_core history of 2 iterations
};

int main() {
    using namespace std;

    fibnobase_gen g;

    cout << "/*** PASSED fibnobase_test() ****/" << endl;

    return 0;
}
