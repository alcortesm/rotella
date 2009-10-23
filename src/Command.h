#ifndef COMMAND_H
#define COMMAND_H

#include <istream>   // USES input stream for operator>> overloading
#include <ostream>   // USES output stream for operator<< overloading
#include <string>    // USES & HASA-USES string interface
#include <stdexcept> // USES invalid_argument exception
#include <vector>    // HASA vector of strings

// The Command class stores a command that has been issued to rotella.
//
// Commands are created by their string representation, that is typed by the
// user or readed from the configuration file. Syntax errors in the string
// will cause the constructor to throw an invalid_argument exception.
//
// Commands can be inspected field by field by accesors methods. As Commands
// need not be modified, there is no modify methods.
//
// A command will have a verb and some arguments. Depending on the verb there
// can be zero, one or more arguments that can be revealed by rArg(int).
//
// Arguments are indexed by numbers, with 0 being the index of the first
// argument and NumArgs()-1 the index of the last argument. If you try to
// access an argument greater than the last one an invalid_argument exception
// will be thrown.
class Command
{
public:
   static void Test();

   Command(const std::string&) throw (std::invalid_argument);

   const std::string&  rVerb() const;
   unsigned int        NumArgs() const;
   const std::string&  rArg(unsigned int) const throw (std::invalid_argument);

private:
    std::string              mVerb;
    unsigned int             mNumArgs;
    std::vector<std::string> mArg;

    static std::string clean_line(const std::string & rLine);
};

std::ostream& operator<<(std::ostream& os, Command c);
std::istream& operator>>(std::istream& is, Command& c);
bool operator==(Command & a, Command & b);
bool operator!=(Command & a, Command & b);

#endif // COMMAND_H
