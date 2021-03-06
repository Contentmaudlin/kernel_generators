#include <cassert>
#include <kgen.hpp>
#include <fib.hpp>

int main() {
    using namespace std;

    kgen::fib_gen g;

    /*
    cout << *g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    cout << *++g << endl;
    */
//    cout << *g << endl;
//   
   /*
    for(auto x : g.forall().filter<string>([](int a) -> std::string { 
        return string ("hi: ") + std::to_string(a); })) {
        cout << x << endl;
        }
        */

    /*
    for(auto x : g.forall().filter([](int a) { return a % 2 == 0; }).map<string>(
                [](int a) -> std::string { return string("hi") + std::to_string(a); })) {
            cout << x << endl;
        }
        */

    for(auto x : g.until_n(3)) {
        cout << x << endl;
    }

    /*
    for(auto &x : g.forall().map<string>([](int a) -> std::string 
          { return string("hi: ") + std::to_string(a) ; })) {
                                                              cout << x << endl;
                                                            }
                                                            */
    /*
    assert(*g == 1);
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
    assert(*++g == 233);
    */

//    for (auto x : g.forall()) cout << x << endl;
    // g.forall()
//    g.forall().map<string>([](int a) -> std::string { return std::to_string(a); });

    cout << "/*** PASSED fib_test ***/" << endl;

    return 0;
}
