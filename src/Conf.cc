#include "Conf.h"
#include "utils.h"
extern Debug_stream debug;
#include "rotella.h"

#include <limits>
#include <sstream>
#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "netdb.h"

using std::string;
using std::endl;
using std::invalid_argument;
using std::bad_alloc;

int
string2int(std::string & s) throw (invalid_argument) {
    int i;
    char c;
    std::istringstream ss(s);
    if (! (ss >> i) || ss.get(c))
        throw invalid_argument(s + " is not a number");
    return i;
}

uint16_t
int2uint16(int i) throw (invalid_argument) {
    if (i < 0)
        throw invalid_argument("less than 0");
    if (i > 0xFFFF)
        throw invalid_argument("greater than 0xFFFF");
    return (uint16_t) i;
}

void
tolower(char * s) {
    char * cp;
    for (cp=s; *cp; cp++)
        *cp = tolower(*cp);
}

bool
has_colon_slash_slash(const char * s) {
    return (strstr(s, COLON_SLASH_SLASH) != NULL);
}

void
prefix_http_colon_slash_slash(char* * sp) throw (bad_alloc) {
    char * news = (char *) calloc(sizeof(char), strlen(*sp) + strlen(HTTP_PROTO) + strlen(COLON_SLASH_SLASH) + 1);
    if (!news)
        throw std::bad_alloc();
    strcat(news, HTTP_PROTO);
    strcat(news, COLON_SLASH_SLASH);
    strcat(news, *sp);
    free(*sp);
    *sp = news;
}

void
parse_url(const string & url, string & proto, string & host, uint16_t & port, string & path)
    throw (invalid_argument, bad_alloc) {
    char * start;
    const char * end;
    size_t sz;

    const char * proto_start;
    const char * proto_end;
    size_t proto_sz;
    
    const char * host_start;
    const char * host_end;
    size_t host_sz;
    
    const char * port_start;
    const char * port_end;
    size_t port_sz;
    
    const char * path_start;
    const char * path_end;
    size_t path_sz;

    start = strdup(url.c_str());
    if (!start)
        throw std::bad_alloc(); 

    tolower(start);
    if (! has_colon_slash_slash(start))
        prefix_http_colon_slash_slash(&start);
    sz = strlen(start);
    end = start + sz;
//    debug << "start=" << start << endl;

    proto_start = strstr(start, HTTP_PROTO);
    if (proto_start != start) {
        free(start);
        throw invalid_argument(url + ": don't start with \"http\"");
    }
    proto_end = strstr(proto_start, COLON_SLASH_SLASH);
    if (proto_end != proto_start + strlen(HTTP_PROTO)) {
        free(start);
        throw invalid_argument(url + ": \"://\" not found after protocol");
    }
    proto_sz = proto_end - proto_start;
    proto = string(proto_start, proto_sz);
//    debug << "proto=" << proto << endl;

    host_start = proto_end + strlen(COLON_SLASH_SLASH);
    path_end = end;
    path_start = strstr(host_start, URL_PATH_SEPARATOR);
    if (!path_start)
        path_start = path_end;
    path_sz = path_end - path_start;
    if (path_sz == 1)
        path_sz = 0; // if path is "/" there is no path
    path = string(path_start, path_sz);
//    debug << "path="  << path  << endl; 

    port_end = path_start;
    port_start = strstr(host_start, URL_PORT_SEPARATOR);
    if (!port_start) {
        port_start = port_end;
        port = (uint16_t) HTTP_DEFAULT_PORT;
    } else {
        port_sz = port_end - port_start;
        string ports;
        ports = string(port_start+1, port_sz-1); // we don't want the ':' to be part of the port
        try {
            int tmp = string2int(ports);
            port = int2uint16(tmp);
        } catch (invalid_argument & ia) {
            free(start);
            throw;
        }
    }
//    debug << "port="  << port  << endl;

    host_end = port_start;
    host_sz = host_end - host_start;
    if (host_sz == 0) {
        free(start);
        throw invalid_argument(url + ": host name not found");
    }
    host = string(host_start, host_sz);
//    debug << "host="  << host  << endl;

    free(start);
    return;
}

void
Webcache::get_ip_and_addr() const throw (invalid_argument) {
    struct hostent * he;
    he = gethostbyname(host_.c_str());
    if (!he) {
        switch (h_errno) {
            case HOST_NOT_FOUND : throw invalid_argument(host_ + ": host not found");
            case NO_DATA : throw invalid_argument(host_ + ": no IP for this host");
            case NO_RECOVERY : throw invalid_argument("name server error while recovering " + host_);
            case TRY_AGAIN : throw invalid_argument("temporary error of name server while recovering " + host_ + ", try again later");
            default : throw invalid_argument("unknown error with gethostbyname()");
        }
    }
    memcpy(&addr_, &he->h_addr, sizeof(he->h_addr));
    ip_ = string(inet_ntoa(addr_));
    
    struct addrinfo * ai;
    int error;
    /* resolve the domain name into a list of addresses */
    error = getaddrinfo(host_.c_str(), NULL, NULL, &ai);
    if (error != 0)
        throw invalid_argument(gai_strerror(error));

    struct addrinfo * curr_ai;
    struct sockaddr_in * sin;
    for (curr_ai=ai; curr_ai!=NULL; curr_ai=curr_ai->ai_next) {
        if (curr_ai->ai_addr->sa_family == AF_INET) {
            sin = (struct sockaddr_in *) curr_ai->ai_addr;
            memcpy(&addr_, &sin->sin_addr, sizeof(sin->sin_addr));
            freeaddrinfo(ai);
            ip_ = string(inet_ntoa(addr_));
            return;
        }
    }
    freeaddrinfo(ai);
    throw invalid_argument("no ipv4 address found");
}

std::string &
Webcache::ip() const throw (invalid_argument) {
    try {
        if (ip_ == "")
            get_ip_and_addr();
    } catch (invalid_argument & ia) {
        throw;
    }
    return ip_;
}

struct in_addr &
Webcache::addr() const throw (invalid_argument) {
    try {
        if (ip_ == "")
            get_ip_and_addr();
    } catch (invalid_argument & ia) {
        throw;
    }
    return addr_;
}

Webcache::Webcache(const string & url) throw (invalid_argument, bad_alloc)
    : orig_(url), ip_("")
{
    addr_.s_addr=0;
    try {
        parse_url(url, proto_, host_, port_, path_);
    } catch (invalid_argument & ia) {
        throw;
    } catch (bad_alloc & ba) {
        throw;
    }
    port_nbo_ = htons(port_);
    debug << *this << endl;
}

std::ostream &
operator<<(std::ostream & os, Webcache wc) {
    os << "[Webcache orig=" << wc.orig()
        << ", proto=" << wc.proto()
        << ", host=" << wc.host()
        << ", port=" << wc.port()
        << ", path=" << wc.path()
        << "]";
    return os;
}

Conf::Conf(std::string localport, std::string sharepath,
        std::string incomingpath, std::string downloadpath,
        std::string webcache) throw (invalid_argument, bad_alloc)
: sharepath_(sharepath), incomingpath_(incomingpath),
    downloadpath_(downloadpath), webcache_(webcache)
    // localport and localport_nbo initilized bellow
{
    try {
        localport_ = string2int(localport);
    } catch (invalid_argument & ia) {
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


