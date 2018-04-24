#ifndef __KGEN_HPP__
#define __KGEN_HPP__

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <exception>
#include <functional>
#include <memory>

namespace kgen {

    /* main generator class */
    template<typename T, int N = 0>
    class gen {
        static_assert(N >= 0, "N must be positive or zero");
        // subclasses
    private:
        class eog_tag {
        };

        class gen_ref : public std::reference_wrapper<gen> {
        public:
            typedef std::input_iterator_tag iterator_category;
            typedef T value_type;
            typedef T &reference;
            typedef T *pointer;
            typedef ptrdiff_t difference_type;

            const T &operator*() { return *this->get(); }

            gen_ref &operator++() {
                ++this->get();
                return *this;
            }

            gen_ref &operator++(int) {
                ++this->get();
                return *this;
            }

            bool operator==(gen &other) { return this->get() == other; }

            bool operator==(gen_ref &other) { return this->get() == other.get(); }

            bool operator!=(gen &other) { return this->get() != other; }

            bool operator!=(gen_ref &other) { return this->get() != other.get(); }
        };

        class generable {
            gen &g;
        public:
            explicit generable(gen &g_) : g{g_} {}

            gen_ref begin() {
                return gen_ref{g};
            }

            const gen_ref end() {
                return gen_ref {eog_gen()};
            }
        };

        class _lb_base {
        protected:
            int ctr;
            bool can_read;
        public:
            explicit _lb_base(int i) : ctr{i}, can_read{false} {}

            void bump() {
                ++ctr;
                can_read = false;
            }
        };

        template<typename U, int Max = 1>
        class _lookback : public _lb_base {
            static_assert(Max >= 0, "Max must be positive");
        public:

            _lookback() : _lb_base(0) { for (auto &x : buf) x = U{}; }

            explicit _lookback(const U &init) : _lb_base(0) { for (auto &x : buf) x = init; }

            explicit _lookback(const U(&arr)[Max]) : _lb_base(0) {
                for (int i = 1; i <= Max; ++i)
                    buf[i] = arr[i - 1];
            }

            _lookback(_lookback &l) : _lb_base(l.ctr), buf(l.buf) {}

            /* lb_core operator overloading */

            _lookback &operator=(const U &val) {
                buf[_lb_base::ctr % (Max + 1)] = val;
                _lb_base::can_read = true;
                return *this;
            }

            const U &operator[](int i) {
                if (i > 0)
                    throw std::invalid_argument("It's called lookback not lookahead!");
                if (i == 0 && !_lb_base::can_read)
                    throw std::invalid_argument("Can't read from unassigned value!");
                if (-i > Max)
                    throw std::invalid_argument(
                            "Can't lb_core more than " + std::to_string(Max) +
                            " (attempted: " + std::to_string(-i) + ")");
                return buf[(_lb_base::ctr + i + Max + 1) % (Max + 1)];
            }

            const U &operator*() {
                if (!_lb_base::can_read) throw std::invalid_argument("Can't read from unassigned value!");
                return buf[_lb_base::ctr % (Max + 1)];
            }

        private:
            /* lb_core member variables */
            std::array<U, Max + 1> buf;
        };

    protected:
        class reached_eog : public std::exception {
            const char *what() const throw() override {
                return "lol sry";
            }
        };

        class lb_base {
        public:
            void bump() { p->bump(); }

        protected:
            explicit lb_base(_lb_base *p_) : p{p_} {}

            std::shared_ptr<_lb_base> p;
        };

        template<typename U, int Max = 1>
        class lookback : public lb_base {
        public:
            lookback() : lb_base{new _lookback<U, Max>{}} {}

            explicit lookback(const U &init) : lb_base{new _lookback<U, Max>(init)} {}

            explicit lookback(const U(&arr)[Max]) : lb_base{new _lookback<U, Max>(arr)} {}

            /* template<class... Args>
             lookback(Args &&... args) : lb_base{new _lookback<U, Max>(std::forward<Args>(args)...)} {}*/

            lookback &operator=(const U &val) {
                static_cast<_lookback<U, Max>>(*lb_base::p).operator=(val);
                return *this;
            }

            const U &operator[](int i) { return static_cast<_lookback<U, Max>>(*lb_base::p).operator[](i); }

            const U &operator*() { return static_cast<_lookback<U, Max>>(*lb_base::p).operator*(); }
        };

        // typedefs
    public:
        typedef std::input_iterator_tag iterator_category;
        typedef T value_type;
        typedef T &reference;
        typedef T *pointer;
        typedef ptrdiff_t difference_type;

        //  member functions
    private:
        constexpr explicit gen(eog_tag) : eog{true} {}

        void set_next() {
            try {
                val = next();
            } catch (reached_eog &e) { eog = true; }
        }

    protected:
        gen() = default;

        gen(std::initializer_list<lb_base> _lbs) : lbs{_lbs} {}

        explicit gen(const T &init, std::initializer_list<lb_base> _lbs = {}) : lbs{_lbs}, val{init} {}

        explicit gen(const T (&arr)[N], std::initializer_list<lb_base> _lbs = {}) : lbs{_lbs}, val{arr} {}

        virtual T next() {
            throw std::out_of_range("Generator at end, can't read!");
        }

        T prev(int i) { return val[i]; }

    public:
        static gen &eog_gen() {
            static gen terminal{eog_tag{}}; // naming - terminal->eog and eog->eog_ TODO
            return terminal;
        }

        const T &operator*() {
            if (!init) {
                set_next();
                init = true;
            }
            return *val;
        }

        gen &operator++() {
            if (!init) {
                set_next();
                init = true;
            }
            for (auto &&l : lbs) l.get().bump();
            val.bump();
            set_next();
            return *this;
        }

        bool at_eog() { return eog; }

        generable forall() { return generable{*this}; }

        bool operator==(const gen &rhs) { return eog && rhs.eog; }

        bool operator!=(const gen &rhs) { return !eog || !rhs.eog; } // DeMorgan's law, look it up ;^)
        gen(const gen &) = delete;

        gen &operator=(const gen &) = delete;
        // member variables
    private:
        std::vector<lb_base> lbs;
        bool init = false;
        bool eog = false;
        lookback<T, N> val;
    };

}

#endif
