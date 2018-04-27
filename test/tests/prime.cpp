#include <cassert>
#include <kgen.hpp>
#include <prime.hpp>

using namespace std;
using namespace kgen;
int main() {
    
    prime_gen p;
    cout << *p << endl;
    cout << *++p << endl;
    cout << *++p << endl;
    cout << *++p << endl;

    return 0;
}
