#include <iostream>

using namespace std;

template <typename Ret>
class Gn {
	int ctr;
protected:
	virtual Ret iterate(void) = 0;

	// do nothing for now
	Gn(void) : ctr{0} {}

public:
	const Ret yield(void)
	{
		Ret ret_buf = iterate();
		++ctr;
		return ret_buf;
	}
};


class MyGn : public Gn<int> {
	int i;
	int iterate(void)
	{
		return i;
	}
public:
	MyGn(int _i) : Gn{}, i{_i} {}
};

int main(void)
{

	MyGn g{1}; // only ever yield 1

	cout << g.yield() << endl;
	cout << g.yield() << endl;

	return 0;
}
