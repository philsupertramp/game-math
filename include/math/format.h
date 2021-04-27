#pragma once
#include <cstdarg>
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
        nsize  = vsnprintf(buffer, (nsize - size)+2, &fmt[size], vl);
        ret += buffer;
    }
    va_end(vl);
    delete[] buffer;
    return ret;
}
