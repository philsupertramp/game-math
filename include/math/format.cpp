#include "format.h"


std::string strip(const std::string& in) {
    std::string out = in;
    auto space      = out.find(' ');
    while(space != std::string::npos) {
        auto backup = out.substr(0, space);
        backup += out.substr(space + 1, out.size() - backup.size());
        out   = backup;
        space = out.find(' ');
    }
    return out;
}


template<typename Out>
void split(const std::string& s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while(std::getline(iss, item, delim)) { *result++ = item; }
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

template<typename Out>
void split(const std::string& s, Out result) {
    std::istringstream iss(s);
    for(auto item : s) { *result++ = std::string(1, item); }
}

std::vector<std::string> split(const std::string& s) {
    std::vector<std::string> elems;
    split(s, std::back_inserter(elems));
    return elems;
}
