#include <vector>
#include <iostream>

using namespace std;

int main(void)
{
    vector<string> v{10};   // over-provision capacity to prevent resizing

    v.push_back("1");
    v.push_back("2");
    v.push_back("3");

    auto i = v.end() - 1;   // access iterator pointing to last element
    auto j = v.end();       // access iterator pointing to end

    v.push_back("hel");     // modify this boi

    auto p = v.end() - 1;   // access iterator pointing to new last element
    auto q = v.end();       // access iterator pointing to end

    cout << "[BEFORE]: i{v.end() - 1} --> " << *i << endl;  // prints 3
    cout << "[BEFORE]: j{v.end()} --> " << *j << endl;      // prints hel

    cout << "v.push_back(\"hel\");" << endl;

    cout << "[AFTER]: p{v.end() - 1} --> " << *p << endl;   // prints hel
    cout << "[AFTER]: q{v.end()} --> " << *q << endl;       // prints nothing

    cout << "j == q: " << (j == q) << endl;                 // prints 0
    cout << "j == p: " << (j == p) << endl;                 // prints 1

    return 0;
}
