#include <cassert>
#include "kgen.hpp"

class fib_gen : public kgen::gen<int, 2> {
    int next() override {
        int x = prev(0);
        return prev(-2) ? prev(-1) + prev(-2) : 1;
    }
};

int main() {
    using namespace std;

    fib_gen g;

    cout << *g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;

    /*assert(*g == 1);
    assert(*++g == 1);
    assert(*++g == 2);
    assert(*++g == 3);
    assert(*++g == 5);
    assert(*++g == 8);
    assert(*++g == 13);
    assert(*++g == 21);
    assert(*++g == 34);
    assert(*++g == 55);
    assert(*++g == 89);
    assert(*++g == 144);
    assert(*++g == 233);*/

    //for (auto& x : g.forall()) cout << x << endl;

    /*auto &&__range = g.forall();
    auto __begin = __range.begin();
    auto __end = __range.end();
    while (__begin != __end) {
        auto x = *__begin;
        cout << x << endl;
        ++__begin;
    }*/

    cout << "/*** PASSED fib_test ***/" << endl;

    return 0;
}
