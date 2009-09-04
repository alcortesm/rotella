#include "utils.h"

#include <sstream>

int
string2int(std::string & s) throw (std::invalid_argument) {
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
