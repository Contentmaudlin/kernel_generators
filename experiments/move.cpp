#include <string>
#include <iostream>

using namespace std;

int main(void)
{
    string s1{"hello"};

    string s2{move(s1)};

    s1.push_back('3');

    cout << s1 << endl;
    cout << s2 << endl;

    return 0;
}

