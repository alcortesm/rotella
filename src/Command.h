#ifndef COMMAND_H
#define COMMAND_H

#include <istream>
#include <ostream>
#include <string>

#include "utils.h"

class Command {
public:
    Command(const std::string & s);
    class Bad_command {}; // exception class

    std::string verb() const;
    unsigned int nargs() const;
    std::string arg(int) const;

    std::string string_rep() const;

private:
    std::string _verb;
    unsigned int _nargs;
    std::string _arg[3];
};

std::ostream & operator<<(std::ostream & os, Command c);
std::istream & operator>>(std::istream & is, Command & c);
bool operator==(Command & a, Command & b);
bool operator!=(Command & a, Command & b);

#endif // COMMAND_H
