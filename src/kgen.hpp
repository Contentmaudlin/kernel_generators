#ifndef __KGEN_HPP__
#define __KGEN_HPP__

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

namespace kgen {
    /* main generator class */
    template<typename T>
    class gen {
    private:
        /* gen inner classes */

        /* abstract base for lookback class */
        class lb_base {
        protected:
            int ctr;
        public:
            explicit lb_base(int i) : ctr{i} {}

            void bump() {
                ++ctr;
            }
        };

        /* gen member variables */
        std::vector<std::reference_wrapper<lb_base>> lbs;
        bool init = false;
        T val;

    protected:
        /* gen inner classes */
        template<typename U, int Max>
        class lookback : public lb_base {
        public:

            lookback() : lb_base(0), buf(Max, U{}) {}

            explicit lookback(const U &init) : lb_base(0), buf(Max, init) {}

            lookback(lookback &l) : lb_base(l.ctr), buf(l.buf) {}

            /* lookback operator overloading */

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
            /* lookback member variables */
            std::vector<U> buf;
        };

        /* gen member functions and variables */
        gen(std::initializer_list<std::reference_wrapper<lb_base>> _lbs)
                : lbs{_lbs} {}

        virtual T next() = 0;

        /* end-of-generator flag */
        bool end = false;

    public:
        /* input iterator typedefs */
        /* TODO do we need these here?
        typedef std::input_iterator_tag iterator_category;
        typedef T value_type; */

        /* gen operator overloading */
        const T &operator*() {
            if (!init) {
                val = next();
                init = true;
            }
            return val;
        }

        gen &operator++() {
            if (!init) {
                val = next();
                init = true;
            }
            for (auto &&l : lbs) l.get().bump();
            val = next();
            return *this;
        }

        /* gen member functions */
        bool at_end() const { return end; } // TODO handle at_end() in op*, op++
    };

}

#endif
