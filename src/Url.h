#ifndef URL_H
#define URL_H

#include <string>
#include <stdexcept>

#include <netinet/ip.h>


class Url {
    public:
        Url(const std::string & url) throw (std::invalid_argument, std::bad_alloc);
        std::string proto() const { return proto_; };
        uint16_t port() const { return port_; };
        uint16_t port_nbo() const { return port_nbo_; };
        std::string host() const { return host_; };
        struct in_addr & addr() const throw (std::invalid_argument);
        std::string & ip() const throw (std::invalid_argument);
        std::string path() const { return path_; };
        std::string orig() const { return orig_; };
    private:
        std::string orig_;
        std::string proto_;
        uint16_t port_;
        uint16_t port_nbo_;
        std::string host_;
        mutable struct in_addr addr_;
        mutable std::string ip_;
        std::string path_;
        void get_ip_and_addr() const throw (std::invalid_argument);
};
std::ostream & operator<<(std::ostream & os, Url url);
#endif // URL_H
