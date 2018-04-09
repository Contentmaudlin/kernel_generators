typedef int thread_num;

template<typename T>
class kgens::gen<T> {
public:
    class iterator {
        typedef std::input_iterator_tag iterator_category;
        typedef T value_type;
        // lambda passed into until dictates operator++() behavior
    };

    class const_iterator {
        typedef std::input_iterator_tag iterator_category;
        typedef T value_type;
    };

private:
    template<class T, int N>
    class kgens::lookback<T, N> { /* … */ };
};

class kgens::mapped_gen<T> : kgens::gen<T> { // ?
};
