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
	struct Lookback {
		std::vector<Type> circular_buf;

		Lookback(const Type& init) : circular_buf(LookbackMax, init) {}

		// ideally the user can't access this
		const Type& get(int lbi, int ctr) const
		{
			// static_assert that lbi < LookbackMax
			return circular_buf[(LookbackMax + ctr - lbi) % LookbackMax];
		}

		void set(const Type& val, int ctr)
		{
			circular_buf[ctr % LookbackMax] = val;
		}
	};

	template <typename Type, int LookbackMax>
	const Type& lb(const struct Lookback<Type, LookbackMax>& lbv, int lbi)
	{
		return lbv.get(lbi, ctr);
	}

	template <typename Type, int LookbackMax>
	const Type& lset(struct Lookback<Type, LookbackMax>& lbv, const Type& val)
	{
		lbv.set(val, ctr);
		return val;
	}

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
		lset(i, lb(i, 1) + lb(i, 2));
		return lb(i, 0);
	}
public:
	Fib(int _i) : Gn{}, i{_i} {}
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
