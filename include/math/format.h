#pragma once
#include <cstdarg>
#include <sstream>
#include <string>

//missing string printf
//this is safe and convenient but not exactly efficient
inline std::string format(const char* fmt, ...) {
    int size     = 512;
    char* buffer = 0;
    buffer       = new char[size];
    va_list vl;
    va_start(vl, fmt);
    int nsize = vsnprintf(buffer, size, fmt, vl);
    std::string ret(buffer);
    if(size <= nsize) { //fail delete buffer and try again
        delete[] buffer;
        buffer = 0;
        buffer = new char[nsize - size + 2]; //+1 for /0
        nsize  = vsnprintf(buffer, (nsize - size) + 2, &fmt[size], vl);
        ret += buffer;
    }
    va_end(vl);
    delete[] buffer;
    return ret;
}

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


/**
 * from https://stackoverflow.com/a/236803
 */
/** ---------------------------------------------- **/
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
/** ----------------------------------------------- **/