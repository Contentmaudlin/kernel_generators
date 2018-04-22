#include <cassert>
#include <string>
#include <vector>
#include "kgen.hpp"

using namespace std;

class str_tok : public kgen::gen<string> {
public:
    str_tok(string s_, string delim_) : kgen::gen<string>{{pos, delim}}, s{s_}, delims{1, delim_} {}

    str_tok(string s_, initializer_list<string> delims_) : kgen::gen<string>{{pos, delim}}, s{s_}, delims{delims_} {}

    string next() override {
        if (pos[-1] == string::npos) throw reached_eog{};
        pos = string::npos;
        for (auto &d : delims) {
            size_t p = s.find(d, pos[-1] + delim[-1]);
            if (p < *pos) {
                pos = p;
                delim = d.size();
            }
        }
        return s.substr(pos[-1] + delim[-1], *pos - pos[-1] - delim[-1]);
    }

private:
    string s;
    vector<string> delims;
    lookback <size_t> pos; // position of end of previously yielded substring
    lookback <size_t> delim; // size of previously encountered delimiter
};

int main() {
    str_tok g{"Bjarne, Ken, Dennis", ", "};
    //g.forall();
    while (!g.at_eog()) {
        cout << *g << endl;
        ++g;
    }
    /*for (auto &&x : g.forall())
        cout << x << endl;*/
}