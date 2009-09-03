#ifndef URL_H
#define URL_H

#include <string>    // USES, HASA & HASA-USES string interface
#include <stdexcept> // USES invalid_argument & bad_alloc exceptions

#include <netinet/ip.h> // HASA struct in_addr


class Url
{
 public:
   Url(const std::string & url) throw (std::invalid_argument, std::bad_alloc);

   std::string      Proto() const { return mProto; };
   uint16_t         Port() const { return mPort; };
   uint16_t         PortNbo() const { return mPortNbo; };
   std::string      Host() const { return mHost; };
   struct in_addr & rAddr() const throw (std::invalid_argument);
   std::string &    rIp() const throw (std::invalid_argument);
   std::string      Path() const { return mPath; };
   std::string      Orig() const { return mOrig; };

 private:
   std::string mOrig;
   std::string mProto;
   uint16_t    mPort;
   uint16_t    mPortNbo;
   std::string mHost;
   mutable struct in_addr mAddr;
   mutable std::string mIp;
   std::string mPath;
   void get_ip_and_addr() const throw (std::invalid_argument);
};

std::ostream & operator<<(std::ostream & os, Url url);
#endif // URL_H
