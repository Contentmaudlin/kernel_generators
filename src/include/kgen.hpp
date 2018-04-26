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
	using std::array;
	using std::vector;
	using std::input_iterator_tag;

	using std::shared_ptr;
	using std::make_shared;
	using std::static_pointer_cast;

	using std::result_of_t;
	using std::function;
	
	using std::initializer_list;
	using std::reference_wrapper;
	
	using std::to_string;
	
	using std::exception;
	using std::out_of_range;
	using std::invalid_argument;

    class eog_tag {
    };

    template<typename T, int N>
    class gen;

    template<typename T, typename K, int N>
    struct map_gen;
    template<typename T, int N>
    struct filter_gen;
    template<typename T, int N>
    struct until_gen;
    template<typename T, int N>
    struct until_n_gen;

    template<typename T, int N = 0>
    struct abstract_gen {
        virtual const T &operator*() = 0;

        virtual abstract_gen &operator++() = 0;

        virtual bool at_eog() const = 0;

        virtual bool operator==(const abstract_gen &rhs) const = 0;

        virtual bool operator!=(const abstract_gen &rhs) const = 0;
    };

    template<typename T, int N = 0>
    class terminal_gen : public abstract_gen<T, N> {
        bool eog;
    public:
        constexpr explicit terminal_gen() : eog{true} {}

        const T &operator*() override { return T{}; }

        terminal_gen &operator++() override { return *this; }

        bool at_eog() const override { return this->eog; }

        bool operator==(const abstract_gen<T, N> &rhs) const override { return false; }

        bool operator!=(const abstract_gen<T, N> &rhs) const override { return true; }
    };

    template<typename T, int N>
    static abstract_gen<T, N> &eoggen() {
        static terminal_gen<T, N> g{};
        return g;
    }

    template<typename T, int N>
    class gen_ref : public reference_wrapper<abstract_gen<T, N>> {
    public:
        typedef input_iterator_tag iterator_category;
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

        bool operator==(abstract_gen<T, N> &other) { return this->get() == other; }

        bool operator==(gen_ref &other) { return this->get() == other.get(); }

        bool operator!=(abstract_gen<T, N> &other) { return this->get() != other; }

        bool operator!=(gen_ref &other) { return this->get() != other.get(); }
    };

    template<typename T, int N>
    struct until_gen : abstract_gen<T, N> {
        abstract_gen<T, N> &g;

        function<bool(T)> ufun;
        bool stop = false;

        until_gen(abstract_gen<T, N> &g_, function<bool(T)> _ufun)
                : g(g_), ufun{_ufun} {}

        const T &operator*() override {
            if (stop)
                throw out_of_range("until_gen out of range!");
            T t = *g;
            if (ufun(t)) stop = true;
            return t;
        }

        until_gen<T, N> &operator++() override {
            if (stop)
                throw out_of_range("until_gen out of range!");
            ++g;
            if (ufun(*g)) stop = true;
            return *this;
        }

        until_gen<T, N> &eoggen() {
            return kgen::eoggen<T, N>();
        }

        bool at_eog() const override {
            return g.at_eog() || this->stop;
        }

        bool operator==(const abstract_gen<T, N> &rhs) const override {
            return (g.at_eog() || this->stop) && rhs.at_eog();
        }

        bool operator!=(const abstract_gen<T, N> &rhs) const override {
            return !(g.at_eog() || this->stop) || !rhs.at_eog();
        }

        class until_generable {
            until_gen<T, N> g;
        public:
            explicit until_generable(until_gen<T, N> g_) : g{g_} {}

            gen_ref<T, N> begin() {
                if (g.ufun(*g)) g.stop = true;
                return gen_ref<T, N>{g};
            }

            const gen_ref<T, N> &end() {
                static gen_ref<T, N> gr{kgen::eoggen<T, N>()};
                return gr;
            }

            template<typename F>
            typename map_gen<result_of_t<F(T)>, T, N>::map_generable map(F map_fun) {
                map_gen<result_of_t<F(T)>, T, N> m{g, map_fun};
                return typename map_gen<result_of_t<F(T)>, T, N>::map_generable(m);
            }

            typename filter_gen<T, N>::filter_generable filter(function<bool(T)> filter_fun) {
                filter_gen<T, N> f{g, filter_fun};
                return typename filter_gen<T, N>::filter_generable(f);
            }
        };
    };

    template<typename T, int N>
    struct until_n_gen : abstract_gen<T, N> {
        abstract_gen<T, N> &g;

        int count_until = 0;
        int counter = 0;
        bool stop = false;

        until_n_gen(abstract_gen<T, N> &g_, int _count_until)
                : g(g_), count_until{_count_until} {}

        const T &operator*() override {
            if (counter >= count_until)
                throw out_of_range("End of until_n");
            return *g;
        }

        until_n_gen<T, N> &operator++() override {
            if (counter >= count_until)
                throw out_of_range("End of until_n");
            if (++counter == count_until)
                stop = true;
            ++g;
            return *this;
        }

        until_n_gen<T, N> &eoggen() {
            return kgen::eoggen<T, N>();
        }

        bool at_eog() const override {
            return g.at_eog() || this->stop;
        }

        bool operator==(const abstract_gen<T, N> &rhs) const override {
            return (g.at_eog() || this->stop) && rhs.at_eog();
        }

        bool operator!=(const abstract_gen<T, N> &rhs) const override {
            return !(g.at_eog() || this->stop) || !rhs.at_eog();
        }

        class until_n_generable {
            until_n_gen<T, N> g;
        public:
            explicit until_n_generable(until_n_gen<T, N> g_) : g{g_} {}

            gen_ref<T, N> begin() {
                return gen_ref<T, N>{g};
            }

            const gen_ref<T, N> &end() {
                static gen_ref<T, N> gr{kgen::eoggen<T, N>()};
                return gr;
            }

            template<typename F>
            typename map_gen<result_of_t<F(T)>, T, N>::map_generable map(F map_fun) {
                map_gen<result_of_t<F(T)>, T, N> m{g, map_fun};
                return typename map_gen<result_of_t<F(T)>, T, N>::map_generable(m);
            }

            typename filter_gen<T, N>::filter_generable filter(function<bool(T)> filter_fun) {
                filter_gen<T, N> f{g, filter_fun};
                return typename filter_gen<T, N>::filter_generable(f);
            }
        };
    };

    template<typename T, typename K, int N>
    struct map_gen : abstract_gen<T, N> {
        abstract_gen<K, N> &g;

        function<T(K)> map_fun;

        T val;

        map_gen(abstract_gen<K, N> &g_, function<T(K)> _map_fun)
                : g(g_), map_fun{_map_fun} {}

        const T &operator*() override {
            val = map_fun(*g);
            return val;
        }

        map_gen<T, K, N> &operator++() override {
            ++g;
            return *this;
        }

        abstract_gen<T, N> &eoggen() {
            return kgen::eoggen<T, N>();
        }

        bool at_eog() const override {
            return g.at_eog();
        }

        bool operator==(const abstract_gen<T, N> &rhs) const override {
            return g.at_eog() && rhs.at_eog();
        }

        bool operator!=(const abstract_gen<T, N> &rhs) const override {
            return !g.at_eog() || !rhs.at_eog();
        }

    public:
        class map_generable {
            map_gen<T, K, N> g;
        public:
            explicit map_generable(map_gen<T, K, N> g_) : g{g_} {}

            gen_ref<T, N> begin() {
                return gen_ref<T, N>{g};
            }

            const gen_ref<T, N> &end() {
                static gen_ref<T, N> gr{kgen::eoggen<T, N>()};
                return gr;
            }

            template<typename F>
            typename map_gen<result_of_t<F(T)>, T, N>::map_generable map(F map_fun) {
                map_gen<result_of_t<F(T)>, T, N> m{g, map_fun};
                return typename map_gen<result_of_t<F(T)>, T, N>::map_generable(m);
            }

            typename filter_gen<T, N>::filter_generable filter(function<bool(T)> filter_fun) {
                filter_gen<T, N> f{g, filter_fun};
                return typename filter_gen<T, N>::filter_generable(f);
            }
        };
    };

    template<typename T, int N>
    struct filter_gen : public abstract_gen<T, N> {
        abstract_gen<T, N> &g;

        function<bool(T)> filter_fun;

        filter_gen(abstract_gen<T, N> &g_, function<bool(T)> _filter_fun)
                : g(g_), filter_fun{_filter_fun} {}

        const T &operator*() override {
            try { while (!filter_fun(*g)) ++g; }
            catch (out_of_range &e) {}
            return *g;
        }

        filter_gen<T, N> &operator++() override {
            ++g;
            try { while (!filter_fun(*g)) ++g; }
            catch (out_of_range &e) {}
            return *this;
        }

        abstract_gen<T, N> &eoggen() {
            return kgen::eoggen<T, N>();
        }

        bool at_eog() const override {
            return g.at_eog();
        }

        bool operator==(const abstract_gen<T, N> &rhs) const override {
            return g.at_eog() && rhs.at_eog();
        }

        bool operator!=(const abstract_gen<T, N> &rhs) const override {
            return !g.at_eog() || !rhs.at_eog();
        }

    public:
        struct filter_generable {
            filter_gen<T, N> g;
        public:
            explicit filter_generable(filter_gen<T, N> g_) : g{g_} {}

            gen_ref<T, N> begin() {
                return gen_ref<T, N>{g};
            }

            const gen_ref<T, N> &end() {
                static gen_ref<T, N> gr{kgen::eoggen<T, N>()};
                return gr;
            }

            template<typename F>
            typename map_gen<result_of_t<F(T)>, T, N>::map_generable map(F map_fun) {
                map_gen<result_of_t<F(T)>, T, N> m{g, map_fun};
                return typename map_gen<result_of_t<F(T)>, T, N>::map_generable(m);
            }

            typename filter_gen<T, N>::filter_generable filter(function<bool(T)> filter_fun) {
                filter_gen<T, N> f{g, filter_fun};
                return typename filter_gen<T, N>::filter_generable(f);
            }
        };
    };

    template<typename T, int N = 0>
    class gen : public abstract_gen<T, N> {
        static_assert(N >= 0, "N must be positive or zero");
    private:
        class generable {
            abstract_gen<T, N> &g;
        public:
            explicit generable(abstract_gen<T, N> &g_) : g{g_} {}

            gen_ref<T, N> begin() {
                return gen_ref<T, N>{g};
            }

            const gen_ref<T, N> &end() {
                static gen_ref<T, N> gr{kgen::eoggen<T, N>()};
                return gr;
            }

            template<typename F>
            typename map_gen<result_of_t<F(T)>, T, N>::map_generable map(F map_fun) {
                map_gen<result_of_t<F(T)>, T, N> m{g, map_fun};
                return typename map_gen<result_of_t<F(T)>, T, N>::map_generable(m);
            }

            typename filter_gen<T, N>::filter_generable filter(function<bool(T)> filter_fun) {
                filter_gen<T, N> f{g, filter_fun};
                return typename filter_gen<T, N>::filter_generable(f);
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

            explicit _lookback(const U &init) : _lb_base(0) {
                for (auto &x : buf) x = init;
            }

            explicit _lookback(const U(&arr)[Max]) : _lb_base(0) {
                for (int i = 1; i <= Max; ++i) buf[i] = arr[i - 1];
            }

            _lookback(_lookback &l) : _lb_base(l.ctr), buf(l.buf) {}

            /* lb_core operator overloading */

            _lookback &operator=(const U &val) {
                buf[_lb_base::ctr % (Max + 1)] = val;
                _lb_base::can_read = true;
                return *this;
            }

            template<int I>
            const U &prev() {
                static_assert(I < 0, "can only lookback");
                static_assert(-I <= Max);
                return buf[(_lb_base::ctr + I + Max + 1) % (Max + 1)];
            }

            const U &operator[](int i) {
                if (i >= 0)
                    throw invalid_argument("It's called lookback not lookahead!");
                if (-i > Max)
                    throw invalid_argument(
                            "Can't lb_core more than " + to_string(Max) +
                            " (attempted: " + to_string(-i) + ")");
                return buf[(_lb_base::ctr + i + Max + 1) % (Max + 1)];
            }

            const U &operator*() {
                if (!_lb_base::can_read)
                    throw invalid_argument("Can't read from unassigned value!");
                return buf[_lb_base::ctr % (Max + 1)];
            }

        private: /* lb_core member variables */
            array<U, Max + 1> buf;
        };

    protected:
        class reached_eog : public exception {
            const char *what() const throw() override {
                return "lol sry";
            }
        };

        class lb_base {
        public:
            void bump() { p->bump(); }

        protected:
            explicit lb_base(shared_ptr<_lb_base> p_) : p{p_} {}

            shared_ptr<_lb_base> p;

            template<typename U, int Max>
            shared_ptr<_lookback<U, Max>> get() {
                return static_pointer_cast<_lookback<U, Max>>(p);
            }
        };

        template<typename U, int Max = 1>
        class lookback : public lb_base {
        public:
            lookback() : lb_base{make_shared<_lookback<U, Max>>()} {}

            explicit lookback(const U &init) : lb_base{make_shared<_lookback<U, Max>>(init)} {}

            explicit lookback(const U(&arr)[Max]) : lb_base{make_shared<_lookback<U, Max>>(arr)} {}

            lookback &operator=(const U &val) {
                this->template get<U, Max>()->operator=(val);
                return *this;
            }

            template<int I>
            const U &prev() {
                return lb_base::template get<U, Max>()->template prev<I>();
            }

            const U &operator[](int i) { return lb_base::template get<U, Max>()->operator[](i); }

            const U &operator*() { return lb_base::template get<U, Max>()->operator*(); }
        };

		typedef reference_wrapper<lb_base> lb_ref;
        class gen_core {
        public:
            vector<lb_ref> lbs;
            bool init = false;
            bool eog = false;
            lookback<T, N> val;

            gen_core() : lbs{}, val{} {}

            gen_core(bool _eog) : lbs{}, eog{_eog}, val{} {}

            gen_core(const T &_init, initializer_list<lb_ref> _lbs = {})
                    : lbs{_lbs}, init{_init}, val{} {}

            gen_core(const T (&arr)[N], initializer_list<lb_ref> _lbs = {})
                    : lbs{_lbs}, val{arr} {}

            gen_core(initializer_list<lb_ref> _lbs)
                    : lbs{_lbs}, val{} {}
        };

    public: // typedefs
        typedef input_iterator_tag iterator_category;
        typedef T value_type;
        typedef T &reference;
        typedef T *pointer;
        typedef ptrdiff_t difference_type;

    private: // member functions
        constexpr explicit gen(eog_tag) : state{make_shared<gen_core>(true)} {}

        void set_next() {
            try { state->val = next(); }
            catch (reached_eog &e) { state->eog = true; }
        }

    protected:

        gen(initializer_list<lb_ref> _lbs) :
                state{make_shared<gen_core>(_lbs)} {
            state->lbs = _lbs;
            state->val = lookback<T, N>{};
        }

        explicit gen(const T &init,
                     initializer_list<lb_ref> _lbs = {})
                : state{init, _lbs} {}

        explicit gen(const T (&arr)[N],
                     initializer_list<lb_ref> _lbs = {})
                : state{_lbs, arr} {}

        virtual T next() {
            throw out_of_range("Generator at end, can't read!");
        }

        const T &prev(int i) { return state->val[i]; }

        template<int I>
        const T &prev() { return state->val.template prev<I>(); }

    public:
        gen() : state{make_shared<gen_core>()} {}

        gen &eoggen() {
            return kgen::eoggen<T, N>();
        }

        const T &operator*() override {
            if (state->eog)
                throw out_of_range("Generator has reached an end!");
            if (!state->init) {
                set_next();
                state->init = true;
            }
            return *state->val;
        }

        gen &operator++() override {
            if (state->eog)
                throw out_of_range("Generator has reached an end!");
            if (!state->init) {
                set_next();
                state->init = true;
            }
            for (auto &&l : state->lbs) l.get().bump();
            state->val.bump();
            set_next();
            return *this;
        }

        bool at_eog() const override { return state->eog; }

        generable forall() { return generable{*this}; }

        typename until_gen<T, N>::until_generable until(function<bool(T)> f) {
            until_gen<T, N> u{*this, f};
            return typename until_gen<T, N>::until_generable{u};
        }

        typename until_n_gen<T, N>::until_n_generable until_n(int n) {
            until_n_gen<T, N> u{*this, n};
            return typename until_n_gen<T, N>::until_n_generable{u};
        }

        bool operator==(const abstract_gen<T, N> &rhs) const override {
            return this->at_eog() && rhs.at_eog();
        }

        // DeMorgan's law, look it up ;^)
        bool operator!=(const abstract_gen<T, N> &rhs) const override {
            return !this->at_eog() || !rhs.at_eog();
        }

        // gen(const gen &) = delete;

        gen &operator=(const gen &) = delete;

    private: // member variables
        shared_ptr<gen_core> state;
    };
} // namespace kgen

#endif
