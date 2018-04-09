#include <list>
#include <iostream>

using namespace std;

int main(void)
{
    list<string> l{10};     // over-provision capacity to prevent resizing

    l.push_back("1");
    l.push_back("2");
    l.push_back("3");

    auto i = l.end();	    // access iterator pointing to last element
    auto j = i--;           // access iterator pointing to end

    l.push_back("hel");     // modify this boi

    auto p = l.end();       // access iterator pointing to new last element
    auto q = p--;           // access iterator pointing to end

    cout << "[BEFORE]: i{l.end() - 1} --> " << *i << endl;
    cout << "[BEFORE]: j{l.end()} --> " << *j << endl;

    cout << "v.push_back(\"hel\");" << endl;

    cout << "[AFTER]: p{l.end() - 1} --> " << *p << endl;
    cout << "[AFTER]: q{l.end()} --> " << *q << endl;

    cout << "j == q: " << (j == q) << endl;
    cout << "j == p: " << (j == p) << endl;

    return 0;
}
