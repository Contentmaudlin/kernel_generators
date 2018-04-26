#ifndef __FIB_HPP
#define __FIB_HPP

#include <kgen.hpp>

namespace kgen {
class fib_gen : public kgen::gen<unsigned long long, 2> {
    unsigned long long next() override {
        return prev(-2) ? prev(-1) + prev(-2) : 1;
    }
};
}

#endif
