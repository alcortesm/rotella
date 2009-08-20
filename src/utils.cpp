#include <iostream>

#include "utils.h"

#include <sstream>

void
Dprintn(const std::string & s)
{
    if (DEBUG) {
        std::cout << s ;
        std::cout << std::endl ;
    }
    return;
}

void
Dprintn(const int & i)
{
    if (DEBUG) {
        std::cout << i ;
        std::cout << std::endl ;
    }
    return;
}

