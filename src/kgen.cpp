#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

namespace kgen {
    /* main generator class */
    template<typename T>
    class gen {
    private:
        /* inner classes */
        class lb_base { // base, non-generic lookback class, to enable storage of many lookback vars
        protected:
            // keep track of position in buffer
            int ctr;
        public:
            // for lookback cctor
            explicit lb_base(int i) : ctr{i} {}

            // called at end of every op++
            void bump() {
                ++ctr;
            }
        };

        /* member variables */
        std::vector<std::reference_wrapper<lb_base>> lbs;
        bool init = false;
        T val;

    protected:
        /* inner classes */
        template<typename U, int Max>
        class lookback : public lb_base { // interface usable by classes that extend gen to declare lookback vars
        public:
            /* constructors */

            // default ctor
            lookback() : lb_base(0), buf(Max, U{}) {}

            // init buffer ctor
            explicit lookback(const U &init) : lb_base(0), buf(Max, init) {}

            // copy ctor
            lookback(lookback &l) : lb_base(l.ctr), buf(l.buf) {}

            /* operator overloading */

            lookback &operator=(const U &val) {
                buf[lb_base::ctr % Max] = val;
                return *this;
            }

            U &operator[](int i) {
                if (i > 0)
                    throw std::invalid_argument("It's called lookback not lookahead!");
                if (-i > Max)
                    throw std::invalid_argument(
                            "Can't lookback more than " + std::to_string(Max) + " (attempted: " + std::to_string(-i) +
                            ")");
                return buf[(lb_base::ctr + i + Max) % Max];
            }

            U &operator*() {
                return buf[lb_base::ctr % Max];
            }

        private:
            /* member variables */

            std::vector<U> buf;
        };

        /* member functions and variables */

        gen(std::initializer_list<std::reference_wrapper<lb_base>> _lbs)
                : lbs{_lbs} {}

        virtual T next() = 0;

        bool eog = false; // flag to detect whether at end-of-generator

    public:
        /* input iterator typedefs */
        typedef std::input_iterator_tag iterator_category;
        typedef T value_type;

        /* operator overloading */
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

        /* member functions */
        bool done() { return eog; }
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

    lookback<int, 2> x; // x will have a lookback history of 2 iterations
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
