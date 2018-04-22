#ifndef __KGEN_HPP__
#define __KGEN_HPP__

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <exception>
#include <functional>

namespace kgen {

    /* main generator class */
    template<typename T, int N = 0>
    class gen {
        // subclasses
    private:
        class eog_tag {
        };

        static gen &get_eog() {
            static gen terminal{eog_tag{}}; // naming - terminal->eog and eog->eog_ TODO
            return terminal;
        }

	   struct gen_ref :  std::reference_wrapper<gen> { 
		 const T& operator*(void) { return *this->get(); }
		 gen_ref &operator++(void) { ++this->get(); return *this; }
		 bool operator==(gen& other) { return this->get() == other; }
		 bool operator==(gen_ref& other) { return this->get() == other.get(); }
 	 	 bool operator!=(gen& other) { return this->get() != other; }
	 	 bool operator!=(gen_ref& other) { return this->get() != other.get(); }
	   }; 

       class generable {
           friend class gen;

           gen &g;
       public:
           explicit generable(gen &g_) : g{g_} {}

           gen_ref begin() { 
             return gen_ref{g}; 
           }

           const gen_ref end() { 
             return gen_ref {get_eog()} ; // std::reference_wrapper<gen>{ get_eog() }; 
           }
       };

        class lb_base {
        protected:
            int ctr;
            bool can_read;
        public:
            explicit lb_base(int i) : ctr{i}, can_read{false} {}

            void bump() {
                ++ctr;
                can_read = false;
            }
        };

    protected:
        class reached_eog : public std::exception {
            const char *what() const throw() override {
                return "lol sry";
            }
        };

        template<typename U, int Max = 1>
        class lookback : public lb_base {
        public:

            lookback() : lb_base(0) { for (auto &x : buf) x = U{}; }

            explicit lookback(const U &init) : lb_base(0) { for (auto &x : buf) x = init; }

            explicit lookback(const U(&arr)[Max]) : lb_base(0) {
                for (int i = 1; i <= Max; ++i)
                    buf[i] = arr[i - 1];
            }

            lookback(lookback &l) : lb_base(l.ctr), buf(l.buf) {}

            /* lookback operator overloading */

            lookback &operator=(const U &val) {
                buf[lb_base::ctr % (Max + 1)] = val;
                lb_base::can_read = true;
                return *this;
            }

            const U &operator[](int i) {
                if (i > 0)
                    throw std::invalid_argument("It's called lookback not lookahead!");
                if (i == 0 && !lb_base::can_read)
                    throw std::invalid_argument("Can't read from unassigned value!");
                if (-i > Max)
                    throw std::invalid_argument(
                            "Can't lookback more than " + std::to_string(Max) +
                            " (attempted: " + std::to_string(-i) + ")");
                return buf[(lb_base::ctr + i + Max + 1) % (Max + 1)];
            }

            const U &operator*() {
                if (!lb_base::can_read) throw std::invalid_argument("Can't read from unassigned value!");
                return buf[lb_base::ctr % (Max + 1)];
            }

        private:
            /* lookback member variables */
            std::array<U, Max + 1> buf;
        };

        // typedefs
    private:
        typedef std::initializer_list<std::reference_wrapper<lb_base>> lb_list;
    public:
        typedef std::input_iterator_tag iterator_category;
        typedef T value_type;

        //  member functions
    private:
        constexpr explicit gen(eog_tag) : eog{true} {}

        static const gen &eog_gen() {
            static gen terminal{eog_tag{}}; // TODO naming
            return terminal;
        }

        void set_next() {
            try {
                val = next();
            } catch (reached_eog &e) { eog = true; }
        }

    protected:
        gen() = default;

        gen(lb_list _lbs) : lbs{_lbs} {}

        explicit gen(const T &init, lb_list _lbs = {}) : lbs{_lbs}, val{init} {}

        explicit gen(const T (&arr)[N], lb_list _lbs = {}) : lbs{_lbs}, val{arr} {}

        virtual T next() {
            throw std::out_of_range("Generator at end, can't read!");
        }

        T prev(int i) { return val[i]; }

    public:

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
        gen (const gen&) = delete;

        gen& operator=(const gen&) = delete;
        // member variables
    private:
        std::vector<std::reference_wrapper<lb_base>> lbs;
        bool init = false;
        bool eog = false;
        lookback<T, N> val;
    };

}

#endif
