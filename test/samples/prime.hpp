#include "kgen.hpp"

class prime_gen : public kgen::gen<int> {
public:
    prime_gen() : kgen::gen<int>{} { x=1; }    // seed state
private:
    bool isPrime(int x) {
      for (int i = 2; i <= x / 2; ++i)
        if (x % i == 0)
          return false;
      return true;
    }

    int next() override {
      while (++x)
        if (isPrime(x))
          return x;
    }

    int x;
};
