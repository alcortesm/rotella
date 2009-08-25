#ifndef WEBCACHE_H
#define WEBCACHE_H

#include <stdexcept>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

class Webcache {
    public:
        Webcache(const std::string & url) throw (std::invalid_argument, std::bad_alloc);
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
std::ostream & operator<<(std::ostream & os, Webcache wc);

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
        const Webcache & webcache() const { return webcache_; };
    private:
        int localport_;
        uint16_t localport_nbo_;
        std::string sharepath_;
        std::string incomingpath_;
        std::string downloadpath_;
        Webcache    webcache_;
};
std::ostream & operator<<(std::ostream & os, Conf c);

#endif // WEBCACHE_H
