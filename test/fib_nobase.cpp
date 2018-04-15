#include "kgen.hpp"

class fibnobase_gen : public kgen::gen<int> {
public:
    fibnobase_gen() : kgen::gen<int>{{x}} {}    // seed state
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

		assert(*g == 2);
		assert(*g++ == 3);
		assert(*g++ == 5);
		assert(*g++ == 8);
		assert(*g++ == 13);
		assert(*g++ == 21);
		assert(*g++ == 34);
		assert(*g++ == 55);
		assert(*g++ == 89);
		assert(*g++ == 144);
		assert(*g++ == 233);

    cout << "/*** PASSED fibnobase_test() ****/" << endl;

    return 0;
}
