#ifndef CONF_H
#define CONF_H

#include "Url.h"

#include <stdexcept>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

class Conf {
    public:
        Conf(std::string localport, std::string sharepath,
                std::string incomingpath, std::string downloadpath,
                std::string webcache) throw (std::invalid_argument, std::bad_alloc);
        int localport() const { return localport_; };
        uint16_t localport_nbo() const { return localport_nbo_; };
        const std::string & sharepath() const { return sharepath_; };
        const std::string & incomingpath() const { return incomingpath_; };
        const std::string & downloadpath() const { return downloadpath_; };
        const Url & webcache() const { return webcache_; };
    private:
        int         localport_;
        uint16_t    localport_nbo_;
        std::string sharepath_;
        std::string incomingpath_;
        std::string downloadpath_;
        Url         webcache_;
};
std::ostream & operator<<(std::ostream & os, Conf c);

#endif // CONF_H
