#ifndef UTILS_H
#define UTILS_H

#include <ostream>

/* prevent compiler warnings about unused variables */
#define UNUSED(x) ((void)(x))


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
