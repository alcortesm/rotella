#include "Command.h"
#include "utils.h"

Command::Command(const std::string& rCommandStr) throw (std::invalid_argument)
{
   mVerb = std::string("Verbo");
   mNumArgs = 2;
   mArg[0] = "Arg1";
   mArg[1] = "Arg2";
   mArg[2] = "";
   UNUSED(rCommandStr);
}

const std::string&
Command::rVerb() const
{
   return mVerb;
}

unsigned int
Command::NumArgs() const
{
   return mNumArgs;
}

const std::string&
Command::rArg(unsigned int i) const throw (std::invalid_argument)
{
   return mArg[i];
}

std::ostream &
operator<<(std::ostream & os, Command c)
{
   os << c.rVerb();
   for (unsigned int i=0; i<c.NumArgs(); i++) os << " " << c.rArg(i);
   return os;
}

std::istream & operator>>(std::istream & is, Command & c)
{
   UNUSED(c);
   return is;
}

bool
operator==(Command & a, Command & b)
{
   UNUSED(a);
   UNUSED(b);
   return true;
}

bool
operator!=(Command & a, Command & b)
{
   return !(a==b);
}
