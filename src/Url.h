// -*- Mode: c++ -*-
#ifndef URL_H
#define URL_H

#include <string>    // USES, HASA & HASA-USES string interface
#include <stdexcept> // USES runtime_error, invalid_argument & bad_alloc exceptions

#include <netinet/ip.h> // HASA struct in_addr

#define HTTP_PROTO "http"
#define COLON_SLASH_SLASH "://"
#define URL_PORT_SEPARATOR ":"
#define URL_PATH_SEPARATOR "/"
#define HTTP_DEFAULT_PORT 80

class Url
{
public:
   class MalformedUrlException : public std::runtime_error {
   public:
      MalformedUrlException() : std::runtime_error("Malformed URL") { }
      MalformedUrlException(std::string s) : std::runtime_error("Malformed URL: " + s) { }
   };

   Url(const std::string & url) throw (Url::MalformedUrlException, std::bad_alloc);

   const std::string &    Proto() const { return mProto; };
   uint16_t               Port() const { return mPort; };
   uint16_t               PortNbo() const { return mPortNbo; };
   const std::string &    Host() const { return mHost; };
   const struct in_addr & Addr() const throw (std::runtime_error);
   const std::string &    Ip() const throw (std::runtime_error);
   const std::string &    Path() const { return mPath; };
   const std::string &    Canonical() const {return mCanonical; };

private:
   std::string            mProto;
   uint16_t               mPort;
   uint16_t               mPortNbo;
   std::string            mHost;
   mutable struct in_addr mAddr;
   mutable std::string    mIp;
   std::string            mPath;
   std::string            mCanonical;

   void get_ip_and_addr() const throw (std::runtime_error);
};

std::ostream & operator<<(std::ostream & os, Url url);
#endif // URL_H
