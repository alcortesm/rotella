// -*- Mode: c++ -*-
#ifndef URL_H
#define URL_H

#include <string>    // USES, HASA & HASA-USES string interface
#include <stdexcept> // USES runtime_error & bad_alloc exceptions

#include <netinet/ip.h> // HASA struct in_addr

#define HTTP_PROTO "http"
#define COLON_SLASH_SLASH "://"
#define URL_PORT_SEPARATOR ":"
#define URL_PATH_SEPARATOR "/"
#define URL_QUERY_SEPARATOR "?"
#define URL_ANCHOR_SEPARATOR "#"
#define HTTP_DEFAULT_PORT 80

class Url
{
public:
   class MalformedUrlException : public std::runtime_error {
   public:
      MalformedUrlException() : std::runtime_error("Malformed URL") { }
      MalformedUrlException(std::string s) : std::runtime_error("Malformed URL: " + s) { }
   };

   class NameResolutionException : public std::runtime_error {
   public:
      NameResolutionException() : std::runtime_error("Name resolution error") { }
      NameResolutionException(std::string s) : std::runtime_error("Name resolution error: " + s) { }
   };

   class NetworkException : public std::runtime_error {
   public:
      NetworkException() : std::runtime_error("Network error") { }
      NetworkException(std::string s) : std::runtime_error("Network error: " + s) { }
   };

   Url(const std::string & rUrl) throw (Url::MalformedUrlException, std::bad_alloc);
   static Url FromParts(const std::string & rProto,
       const std::string & rHost,
       const std::string & rPath,
       const uint16_t      port = 0,
       const std::string & rQuery  = "",
       const std::string & rAnchor = "") throw (Url::MalformedUrlException, std::bad_alloc);

   const std::string &    Proto() const { return mProto; };
   uint16_t               Port() const { return mPort; };
   uint16_t               PortNbo() const { return mPortNbo; };
   const std::string &    Host() const { return mHost; };
   const struct in_addr & Addr() const throw (Url::NameResolutionException, Url::NetworkException);
   const std::string &    Ip() const throw (Url::NameResolutionException, Url::NetworkException);
   const std::string &    Path() const { return mPath; };
   const std::string &    Query() const { return mQuery; };
   const std::string &    Anchor() const { return mAnchor; };
   const std::string &    Canonical() const { return mCanonical; };
   bool                   HasQuery() const { return ! mQuery.empty(); };
   bool                   HasAnchor() const { return ! mAnchor.empty(); };

   static void            test(void);

private:
   std::string            mProto;
   uint16_t               mPort;
   uint16_t               mPortNbo;
   std::string            mHost;
   mutable struct in_addr mAddr;
   mutable std::string    mIp;
   std::string            mPath;
   std::string            mQuery;
   std::string            mAnchor;
   std::string            mCanonical;

   void get_ip_and_addr() const throw (Url::NameResolutionException, Url::NetworkException);
};

std::ostream & operator<<(std::ostream & os, Url url);
#endif // URL_H
