#include "Conf.h"
#include "utils.h"

#include <iostream>
#include <string.h>
#include <stdexcept>

Conf::Conf(std::string localport, std::string sharepath,
        std::string incomingpath, std::string downloadpath,
           std::string webcache) throw (std::invalid_argument, std::bad_alloc)
: sharepath_(sharepath), incomingpath_(incomingpath),
    downloadpath_(downloadpath), webcache_(webcache)
    // localport and localport_nbo initilized bellow
{
    try {
        localport_ = string2int(localport);
    } catch (std::invalid_argument & ia) {
        throw ;
    }
    localport_nbo_ = htons(localport_);
}

std::ostream &
operator<<(std::ostream & os, Conf c) {
    os << "[Conf: localport=" << c.localport()
        << ", sharepath=" << c.sharepath()
        << ", incomingpath=" << c.incomingpath()
        << ", downloadpath=" << c.downloadpath()
        << ", webcache=" << c.webcache()
        << "]";
    return os;
}


