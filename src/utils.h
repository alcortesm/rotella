// -*- Mode: c++ -*-
#ifndef UTILS_H
#define UTILS_H

#include <ostream>
#include <string>
#include <stdexcept>

/* prevent compiler warnings about unused variables */
#define UNUSED(x) ((void)(x))

// string and int conversions
int         string_to_int(std::string & s) throw (std::invalid_argument);
std::string uint16_to_string(uint16_t i);
uint16_t    int_to_uint16(int i) throw (std::invalid_argument);

// c string handling
void to_lower(char * s);

// Stream to use for debugging
// It is thread safe, but funny things can happend
// with on(). Example of use:
// #include "debugstream.h"
// DebugStream debug(std::cout);
// int main() {
//   debug << "Debug" << std::endl;
//   debug.on() = false;
//   debug << "This line will not print" << std::endl;
//   return 0;
// }
class Debug_stream {
    std::ostream & stream;
    bool ison;
    
    public:
    
    Debug_stream(std::ostream & _stream, bool _ison = true)
        : stream(_stream), ison(_ison) { }

    template <typename T> inline Debug_stream & operator<<(const T & t) {
        if (ison)
            stream << t;
        return *this;
    }
    
    inline Debug_stream & operator<<(std::ostream& (*f)(std::ostream &)) {
        if (ison)
            stream << f;
        return *this;
    }

    bool on() const {
        return ison;
    }

    bool& on() {
        return ison;
    }
};

#endif
