#include "utils.h"

#include <sstream>
#include <cerrno>
#include <cstring>

int
string_to_int(std::string & s) throw (std::invalid_argument) {
    int i;
    char c;
    std::istringstream ss(s);
    if (! (ss >> i) || ss.get(c))
       throw std::invalid_argument(s + " is not a number");
    return i;
}

std::string
uint16_to_string(uint16_t i)
{
   std::ostringstream oss;
   oss<< i;
   return oss.str();
}

uint16_t
int_to_uint16(int i) throw (std::invalid_argument) {
   if (i < 0)
      throw std::invalid_argument("less than 0");
   if (i > 0xFFFF)
      throw std::invalid_argument("greater than 0xFFFF");
   return (uint16_t) i;
}

void
to_lower(char * s) {
   char * cp;
   for (cp=s; *cp; cp++)
      *cp = tolower(*cp);
}

void
throw_fname_errno(const char * fname)
   throw (std::runtime_error)
{
   throw std::runtime_error(std::string(fname) + "(): " + strerror(errno));
}

void
throw_fname(const char * fname, const char * reason)
   throw (std::runtime_error)
{
   throw std::runtime_error(std::string(fname) + "(): " + reason);
}
