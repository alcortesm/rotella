#ifndef COMMAND_H
#define COMMAND_H

#include <istream>
#include <ostream>
#include <string>

class Command
{
public:
   Command(const std::string&);
   class BadCommand {}; // exception class

   const std::string&  rVerb() const;
   unsigned int        NumArgs() const;
   const std::string&  rArg(int) const;

private:
    std::string  mVerb;
    unsigned int mNumArgs;
    std::string  mArg[3];
};

std::ostream& operator<<(std::ostream& os, Command c);
std::istream& operator>>(std::istream& is, Command& c);
bool operator==(Command & a, Command & b);
bool operator!=(Command & a, Command & b);

#endif // COMMAND_H
