#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

namespace kgen {

    template<typename T>
    class gen {
    private:
        class _lookback {
        protected:
            int ctr;
        public:
            explicit _lookback(int i) : ctr{i} {}

            void bump() {
                ++ctr;
            }
        };

        std::vector<std::reference_wrapper<_lookback>> lbs;
        bool init = false;
        T val;

    protected:
        template<typename U, int Max>
        class lookback : public _lookback {
        public:
            std::vector<U> circular_buf;

            lookback() : _lookback(0), circular_buf(Max, U{}) {}

            explicit lookback(const U &init) : _lookback(0), circular_buf(Max, init) {}

            lookback(lookback &l) : _lookback(l.ctr), circular_buf(l.circular_buf) {}

            lookback &operator=(const U &val) {
                circular_buf[_lookback::ctr % Max] = val;
                return *this;
            }

            U &operator[](int i) {
                if (i > 0)
                    throw std::invalid_argument("It's not called lookback not lookahead!");
                if (-i > Max)
                    throw std::invalid_argument(
                            "Can't lookback more than " + std::to_string(Max) + " (attempted: " + std::to_string(-i) +
                            ")");
                return circular_buf[(_lookback::ctr + i + Max) % Max];
            }

            U &operator*() {
                return circular_buf[_lookback::ctr % Max];
            }
        };

        virtual T next() = 0;

        gen(std::initializer_list<std::reference_wrapper<_lookback>> _lbs)
                : lbs{_lbs} {}

    public:
        typedef std::input_iterator_tag iterator_category;
        typedef T value_type;

        const T operator*() {
            if (!init) {
                val = next();
                init = true;
            }
            return val;
        }

        gen &operator++() {
            for (auto &&l : lbs) l.get().bump();
            val = next();
            return *this;
        }

        gen &operator++(int) {
            return operator++();
        }
    };
}

class fib_gen : public kgen::gen<int> {
public:
    fib_gen() : kgen::gen<int>{{x}} {}    // seed state
private:
    int next() override {
        x = x[-1] + x[-2];
        return *(*x == 0 ? x = 1 : x);
    }

    lookback<int, 2> x{0}; // x will have a lookback history of 2 iterations
};


int main() {

    fib_gen g;
    //Fib k = g;

    using namespace std;

    cout << *g << endl;
    cout << *g++ << endl;
    cout << *g++ << endl;
    cout << *g++ << endl;
    cout << *g++ << endl;
    cout << *g++ << endl;
    cout << *g++ << endl;
    cout << *g++ << endl;
    cout << *g++ << endl;
    cout << *g++ << endl;
    cout << *g++ << endl;

    cout << endl;

//    cout << *k << endl;
//    cout << *k << endl;
//    cout << *k << endl;
//    cout << *k << endl;
//    cout << *k << endl;

    return 0;
}
