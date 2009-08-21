#include "Command.h"

Command::Command(const std::string & s) {
    _verb = std::string("Verbo");
    _nargs = 2;
    _arg[0] = "Arg1";
    _arg[1] = "Arg2";
    _arg[2] = "";
    UNUSED(s);
}

std::string
Command::verb() const {
    return _verb;
}

unsigned int
Command::nargs() const {
    return _nargs;
}

std::string
Command::arg(int i) const {
    return _arg[i];
}

std::ostream &
operator<<(std::ostream & os, Command c) {
    os << c.verb();
    for (unsigned int i=0; i<c.nargs(); i++)
        os << " " << c.arg(i);
    return os;
}

std::istream & operator>>(std::istream & is, Command & c) {
    UNUSED(c);
    return is;
}

bool
operator==(Command & a, Command & b) {
    UNUSED(a);
    UNUSED(b);
    return true;
}

bool
operator!=(Command & a, Command & b) {
    return !(a==b);
}


