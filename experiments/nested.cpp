#include <iostream>

using namespace std;


class Kn {
    struct Lb {
        const Kn& p;

	int x;
        Lb(const Kn& k, int i) : p{k}, x{i} {}

        void dong()
        {
            cout << "dong: " << i << endl;
        }
    };

    Lb l;
protected:

public:

    int i;
    Kn(int _i) : l{this}, i{i}{}

    void ba()
    {

        i++;
        cout << "ba!!" << endl; 

        l.dong();
    }

};


class Kn2 : Kn {
	Kn::Lb l2;


	Kn2(int x) : l2{*this, x} {}

	int iterate(int param)
	{
		

	}
}



int main(void)
{
    Kn k{3};
    k.ba();
    k.ba();

    return 0;
}
