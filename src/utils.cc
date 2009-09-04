#include "utils.h"

#include <sstream>

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

