// -*- Mode: c++ -*-
#ifndef CONF_H
#define CONF_H

#include "Url.h"      // HASA url for the WebCache

#include <stdexcept>  // USES invalid_argument and bad_alloc
#include <string>     // HASA some strings

class Conf {
public:
   Conf(std::string & localPort, std::string & sharePath,
        std::string & incomingPath, std::string & downloadPath,
        std::string & webCache) throw (std::invalid_argument, std::bad_alloc);
   Conf(const char* localPort, const char* sharePath, const char* incomingPath,
        const char* downloadPath, const char* webCache) throw (std::invalid_argument, std::bad_alloc);
   int LocalPort() const { return mLocalPort; };
   uint16_t localport_nbo() const { return mLocalPortNbo; };
   const std::string & sharepath() const { return mSharePath; };
   const std::string & incomingpath() const { return mIncomingPath; };
   const std::string & downloadpath() const { return mDownloadPath; };
   const Url & webcache() const { return mWebCache; };
private:
   int         mLocalPort;
   uint16_t    mLocalPortNbo;
   std::string mSharePath;
   std::string mIncomingPath;
   std::string mDownloadPath;
   Url         mWebCache;
};
std::ostream & operator<<(std::ostream & os, Conf c);

#endif // CONF_H
