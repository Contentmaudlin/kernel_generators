#ifndef __PRIME_HPP
#define __PRIME_HPP

#include <kgen.hpp>

namespace kgen {
class prime_gen : public kgen::gen<unsigned long long, 1> {
    bool is_prime(int n) {
        if (n < 2)
            return false;
        for (int i = 2; i < n; ++i)
            if (n % i == 0)
                return false;
        return true;
    }
    unsigned long long next() override {
        int last{++prev(-1)};
        while (!is_prime(++last));
        return last;
    }
};
}

#endif
