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

  /*
    template<typename U, int N = 0>
    class gen {

      gen(std::initializer_list<std::reference_wrapper<lb_base>>_lbs) : p{new gen{_lbs}} { }

      explicit gen(const T &init, std::initializer_list<std::reference_wrapper<lb_base>> _lbs = {}) : 
        p{new gen{init, _lbs}} { }

      explicit gen(const T (&arr)[N], std::initializer_list<std::reference_wrapper<lb_base>> _lbs = {}) : 
        p{new gen{arr, _lbs}} { }

      shared_ptr<gen> p;

    }
    */

    class eog_tag { };

    template<typename T, int N> class gen;

    template<typename T, int N = 0>
    struct abstract_gen {
      virtual T next() = 0; 
      virtual T prev(int) = 0;
      virtual const T operator*() = 0;
      virtual abstract_gen &operator++() = 0;
      virtual bool at_eog() const = 0;
      virtual bool operator==(const abstract_gen &rhs) const = 0;
      virtual bool operator!=(const abstract_gen &rhs) const = 0;
      virtual abstract_gen &eoggen() = 0;
    };

    template<typename T, int N>
    class gen_ref : public std::reference_wrapper<abstract_gen<T, N>> {
    public:
      typedef std::input_iterator_tag iterator_category;
      typedef T value_type;
      typedef T &reference;
      typedef T *pointer;
      typedef ptrdiff_t difference_type;

      const T operator*() { return *this->get(); }

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

    template<typename K, typename L, int X>
    struct map_gen : abstract_gen<K, X> {
      abstract_gen<L, X> &g;

      std::function<K(L)> map_fun;

      map_gen(abstract_gen<L, X> &g_, std::function<K(L)> _map_fun)
        : g(g_), map_fun{_map_fun} { }

      constexpr explicit map_gen(eog_tag) : 
        g{*std::make_shared<gen<L,X>>(gen<L,X>{})} { }

      K next() override {
        return map_fun(g.next());
      }

      K prev(int i) override {
        return map_fun(g.prev(i));
      }

      const K operator*() override {
        L t = *g;
        return map_fun(t);
      }

      map_gen<K, L, X> &operator++() override {
        ++g;
        return *this;
      }

      map_gen<K, L, X> &eoggen() override {
          map_gen<K, L, X> terminal{eog_tag{}};
          return terminal;
      }

      bool at_eog() const override {
        return g.at_eog();
      }

      bool operator==(const abstract_gen<K, X> &rhs) const override {
        return g.at_eog() && rhs.at_eog();
      }

      bool operator!=(const abstract_gen<K,X> &rhs) const override {   
        return !g.at_eog() || !rhs.at_eog();
      }

      public:
      class map_generable {
        map_gen<K, L, X> g;
      public:
          explicit map_generable(map_gen<K, L, X> g_) : g{g_} { } 

          /*
          template <typename K>
          explicit generable<K, N>(map_gen<T, K, N> &&g_) 
          : g{std::make_shared<abstract_gen<T, N>>(g_)} { }
          */

          gen_ref<K, X> begin() {
              return gen_ref<K, X>{g};
          }

          const gen_ref<K, X> &end() {
            return gen_ref<K, X>{g.eoggen()};
          }

          /*
          template<typename K>
          generable map(std::function<K(T)> map_fun) {
            map_gen<K, T, N> m{g, map_fun};
            return generable{m};
          }
          */
      };

   };

    template<typename T, int N = 0>
    class gen : public abstract_gen<T, N> {
        static_assert(N >= 0, "N must be positive or zero");
        // subclasses
    private:
      class generable {
        abstract_gen<T, N> &g;
      public:
          explicit generable(abstract_gen<T, N> &g_) : g{g_} { } 

          /*
          template <typename K>
          explicit generable<K, N>(map_gen<T, K, N> &&g_) 
          : g{std::make_shared<abstract_gen<T, N>>(g_)} { }
          */

          gen_ref<T, N> begin() {
              return gen_ref<T, N>{g};
          }

          const gen_ref<T, N> &end() {
            return gen_ref<T, N>{g.eoggen()};
          }

          template<typename K>
          typename map_gen<K, T, N>::map_generable map(std::function<K(T)> map_fun) {
            map_gen<K, T, N> m{g, map_fun};
            return typename map_gen<K, T, N>::map_generable(m);
          }

          /*
          template<typename K>
          generable map(std::function<K(T)> map_fun) {
            map_gen<K, T, N> m{g, map_fun};
            return generable{m};
          }
          */
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

          const U operator*() {
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
            explicit lb_base(std::shared_ptr<_lb_base> p_) : p{p_} {}

            std::shared_ptr<_lb_base> p;

            template<typename U, int Max>
            std::shared_ptr<_lookback<U, Max>> get() {
                return std::static_pointer_cast<_lookback<U, Max>>(p);
            }
        };

      template<typename U, int Max = 1>
      class lookback : public lb_base {
      public:
          lookback() : lb_base{std::make_shared<_lookback<U, Max>>() } {}

          explicit lookback(const U &init) : lb_base{std::make_shared<_lookback<U, Max>>(init)} {}

          explicit lookback(const U(&arr)[Max]) : lb_base{std::make_shared<_lookback<U, Max>>(arr)} {}

          /* template<class... Args>
           lookback(Args &&... args) : lb_base{new _lookback<U, Max>(std::forward<Args>(args)...)} {}*/

          lookback &operator=(const U &val) {
              this->template get<U, Max>()->operator=(val);
              //static_pointer_cast<std::shared_ptr<_lookback<U, Max>>>(lb_base::p)->operator=(val);
              return *this;
          }

          const U &operator[](int i) { return lb_base::template get<U, Max>()->operator[](i); }

          const U operator*() { return lb_base::template get<U, Max>()->operator*(); }
      };

      class gen_core {
        public:
        std::vector<std::reference_wrapper<lb_base>> lbs;
        bool init = false;
        bool eog = false;
        lookback<T, N> val;
        gen_core() : lbs{}, val{} { } 
        gen_core(bool _eog) : lbs{}, eog{_eog}, val{} { } 
        gen_core(const T& _init, std::initializer_list<std::reference_wrapper<lb_base>> _lbs = {})
          : lbs{_lbs}, init{_init},val{} {} 
        gen_core(const T (&arr)[N], std::initializer_list<std::reference_wrapper<lb_base>> _lbs = {})
          : lbs{_lbs}, val{arr} { }
        gen_core(std::initializer_list<std::reference_wrapper<lb_base>> _lbs)
          : lbs{_lbs}, val{ } { } 
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
        constexpr explicit gen(eog_tag) : state{std::make_shared<gen_core>(true)} {} 

        void set_next() {
            try {
                state->val = next();
            } catch (reached_eog &e) { state->eog  = true; }
        }

      protected:

        gen(std::initializer_list<std::reference_wrapper<lb_base>>_lbs) : 
          state{std::make_shared<gen_core>(_lbs)}  { 
            state->lbs = _lbs;
            state->val = lookback<T, N>{}; 
          }

        explicit gen(const T &init, std::initializer_list<std::reference_wrapper<lb_base>> _lbs = {}) 
          : state{init, _lbs} { }

        explicit gen(const T (&arr)[N], std::initializer_list<std::reference_wrapper<lb_base>> _lbs = {}) 
          : state{_lbs, arr} { }

        virtual T next() override {
            std::cout << "gen next() " << std::endl;
            throw std::out_of_range("Generator at end, can't read!");
        }

        T prev(int i) override { return state->val[i]; }

      public:
        gen() : state{std::make_shared<gen_core>()} { }

        gen &eoggen() override {
          gen terminal{eog_tag{}};
          return terminal;
        }
        const T operator*() override {
            if (!state->init) {
                set_next();
                state->init = true;
            }
            return *state->val;
        }

        gen &operator++() override {
            if (!state->init) {
                set_next();
                state->init = true;
            }
            for (auto &&l : state->lbs) l.get().bump();
            state->val.bump();
            set_next();
            return *this;
        }

        bool at_eog() const override { 
          return state->eog; 
        }

        generable forall() {
          return generable{*this};
        }

        bool operator==(const abstract_gen<T, N> &rhs) const override { 
          return this->at_eog() && rhs.at_eog();
        }
         
        // DeMorgan's law, look it up ;^)
        bool operator!=(const abstract_gen<T, N> &rhs) const override { 
          return !this->at_eog() || !rhs.at_eog();
        }

 //       gen(const gen &) = delete;

        gen &operator=(const gen &) = delete;

        // member variables
    private:
        std::shared_ptr<gen_core> state;
        /*
        std::vector<std::reference_wrapper<lb_base>> lbs;
        bool init = false;
        bool eog = false;
        lookback<T, N> val;
        */
    };



//    template<typename T, int N>
//    generable<T,N> gen<T, N>::forall() { return generable{*this}; }
}

#endif
