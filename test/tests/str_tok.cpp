#include <cassert>
#include <string>
#include <vector>
#include <kgen.hpp>
#include <str_tok.hpp>

using namespace std;

int main() {
    kgen::str_tok g{"Bjarne, Ken, Dennis", ", "};
    vector<string> v(g.forall().begin(), g.forall().end());
    for (auto &x : v) cout << x << endl;
//    cout << *g << endl;
//    cout << *++g << endl;
//    cout << *++g << endl;
//    ++g;
//    ++g;
}
