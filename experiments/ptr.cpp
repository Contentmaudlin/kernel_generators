#include <iostream>
#include <memory>

using namespace std;


int main(void)
{
    unique_ptr<int> u1;
    unique_ptr<int> u2{new int{10}};


    //u1 = u2; // compiler complains about deleted function
    u1 = move(u2);

    cout << *u1 << endl;
    cout << *u2 << endl;

    return 0;
}
