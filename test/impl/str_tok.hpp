#ifndef __STR_TOK_HPP
#define __STR_TOK_HPP

#include <string>
#include <vector>
#include <initializer_list>
#include <kgen.hpp>

using std::string;
using std::size_t;
using std::initializer_list;
using std::vector;

namespace kgen {

class str_tok : public gen<string> {
public:
    str_tok(string s_, string delim_) :
        gen<string>{{end_pos, delim_sz}}, s{s_}, delims{1, delim_} {}

    str_tok(string s_, initializer_list<string> delims_)
            : gen<string>{{end_pos, delim_sz}}, s{s_}, delims{delims_} {}

    string next() override {
        //if (end_pos[-1] == string::npos) throw reached_eog{};
        if (end_pos.prev<-1>() == string::npos) throw reached_eog{};

        //size_t start_pos = end_pos[-1] + delim_sz[-1];
        size_t start_pos = end_pos.prev<-1>() + delim_sz.prev<-1>();
        end_pos = string::npos;
        for (auto &d : delims) {
            size_t p = s.find(d, start_pos);
            if (p < *end_pos) {
                end_pos = p;
                delim_sz = d.size();
            }
        }
        return s.substr(start_pos, *end_pos - start_pos);
    }

private:
    string s;
    vector<string> delims;
    lookback<size_t> end_pos;   // position of end of previously yielded substr
    lookback<size_t> delim_sz;  // size of previously encountered delimiter
};
}

#endif
