#include <cassert>
#include <string>
#include <vector>
#include "kgen.hpp"

using namespace std;

class str_tok : public kgen::gen<string> {
public:
    str_tok(string s_, string delim_) : kgen::gen<string>{{end_pos, delim}}, s{s_}, delims{1, delim_} {}

    str_tok(string s_, initializer_list<string> delims_)
            : kgen::gen<string>{{end_pos, delim}}, s{s_}, delims{delims_} {}

    string next() override {
        if (end_pos[-1] == string::npos) throw reached_eog{};
        size_t start_pos = end_pos[-1] + delim[-1];
        end_pos = string::npos;
        for (auto &d : delims) {
            size_t p = s.find(d, start_pos);
            if (p < *end_pos) {
                end_pos = p;
                delim = d.size();
            }
        }
        return s.substr(start_pos, *end_pos - start_pos);
    }

private:
    string s;
    vector<string> delims;
    lookback <size_t> end_pos, delim; // position of end of previously yielded substring and size of previously encountered delimiter
};

int main() {
    for (str_tok g{"Bjarne, Ken, Dennis", ", "}; !g.at_eog(); ++g) cout << *g << endl;
}