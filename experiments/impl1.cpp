#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename Ret>
class Gn {
	int ctr;
protected:
	virtual Ret iterate(void) = 0;

	// do nothing for now
	Gn(void) : ctr{0} {}

	template <typename Type, int LookbackMax>
	class Lookback {
		public:
			std::vector<Type> circular_buf;
			const Gn *g;

			explicit Lookback(const Gn *g, const Type& init) : circular_buf(LookbackMax, init) {
				this->g = g;
			}

			Type& operator=(const Type& val)
			{
				this->circular_buf[g->ctr % LookbackMax] = val;
				return circular_buf[g->ctr % LookbackMax];
			}

			Type& operator[](int i) 
			{
				if (i > 0) 
					throw std::invalid_argument("It's called lookback not lookahead!");
				return circular_buf[(LookbackMax + g->ctr + i) % LookbackMax];
			}
	};

public:
	const Ret yield(void)
	{
		Ret ret_buf = iterate();
		++ctr;
		return ret_buf;
	}
};


class Fib : public Gn<int> {
	Lookback<int, 4> i;
	int iterate(void)
	{
		i = i[-1] + i[-2];
		return i[0];
	}

public:
	Fib(int _i) : Gn{}, i{this, _i} {}
};

int main(void)
{

	Fib g{1}; // only ever yield 1

	cout << g.yield() << endl;
	cout << g.yield() << endl;
	cout << g.yield() << endl;
	cout << g.yield() << endl;
	cout << g.yield() << endl;
	cout << g.yield() << endl;
	cout << g.yield() << endl;
	cout << g.yield() << endl;

	return 0;
}
