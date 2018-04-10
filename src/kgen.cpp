#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename Ret>
class Gn {
protected:
	virtual Ret iterate(void) = 0;

	class LbInternal {
		protected:
			int ctr;
		public:
			LbInternal(int i)
			{
				this->ctr = i;
			}
			void bump() 
			{
				this->ctr++;
			}
	};

	vector<std::reference_wrapper<LbInternal>> lbs;

  Gn(std::initializer_list<std::reference_wrapper<LbInternal>> flist) 
		: lbs(flist) { }

	template <typename Type, int LookbackMax>
	class Lookback : public LbInternal {
		public:
			std::vector<Type> circular_buf;

			Lookback(const Type& init) : LbInternal(0), 
			                             circular_buf(LookbackMax, init) { }

			Type& operator=(const Type& val)
			{
				this->circular_buf[LbInternal::ctr % LookbackMax] = val;
				return circular_buf[LbInternal::ctr % LookbackMax];
			}

			Type &operator[](int i)
			{
				if (i > 0)
					throw std::invalid_argument("It's not called lookback not lookahead!");
				if (i > LookbackMax)
					throw std::invalid_argument("Can't lookback more than " + LookbackMax);
				return circular_buf[(LookbackMax + LbInternal::ctr + i) % LookbackMax];
			}

			Type &operator*()
			{
				return circular_buf[(LookbackMax + LbInternal::ctr) % LookbackMax];
			}

			Lookback(Lookback &l) : LbInternal(l.ctr)
			{
				this->circular_buf = l.circular_buf;
			}
	};

public:
	const Ret operator*(void)
	{
		Ret ret_buf = iterate();
		for (LbInternal &l : lbs)
		{
			l.bump();
		}
		return ret_buf;
	}
};

class Fib : public Gn<int> {
	Lookback<int, 4> i;
	int iterate(void)
	{
		i = i[-1] + i[-2];
		return *i;
	}

public:
	Fib(int _i) : Gn{i}, i{_i} { }
};

int main(void)
{

	Fib g{1}; // only ever yield 1
	Fib k = g;

	cout << *g << endl;
	cout << *g << endl;
	cout << *g << endl;
	cout << *g << endl;
	cout << *g << endl;
	cout << *g << endl;
	cout << *g << endl;
	cout << *g << endl;

	cout << endl;

	return 0;
}
