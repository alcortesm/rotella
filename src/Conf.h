// -*- Mode: c++ -*-
#ifndef CONF_H
#define CONF_H

#include "Url.h"      // HASA url for the WebCache

#include <stdexcept>  // USES invalid_argument and bad_alloc
#include <string>     // HASA some strings

class Conf {
public:
   static Conf FromFile(const std::string & path = "./config") throw (std::invalid_argument, std::bad_alloc);
   int LocalPort() const { return mLocalPort; };
   uint16_t LocalPortNbo() const { return mLocalPortNbo; };
   const std::string & SharePath() const { return mSharePath; };
   const std::string & IncomingPath() const { return mIncomingPath; };
   const std::string & DownloadPath() const { return mDownloadPath; };
   const Url & WebCache() const { return mWebCache; };
private:
   Conf(std::string & localPort, std::string & sharePath,
        std::string & incomingPath, std::string & downloadPath,
        std::string & webCache) throw (std::invalid_argument, std::bad_alloc);
   Conf(const char* localPort, const char* sharePath, const char* incomingPath,
        const char* downloadPath, const char* webCache) throw (std::invalid_argument, std::bad_alloc);
   int         mLocalPort;
   uint16_t    mLocalPortNbo;
   std::string mSharePath;
   std::string mIncomingPath;
   std::string mDownloadPath;
   Url         mWebCache;
};
std::ostream & operator<<(std::ostream & os, Conf c);

#endif // CONF_H
