#include "Conf.h"
#include "utils.h"

#include <iostream>
#include <string>
#include <stdexcept>

// create the configuration variables with
// the values stored in the configuration
// file or defaults where available
/* static */ Conf
Conf::FromFile(const std::string & path) throw (std::invalid_argument, std::bad_alloc)
{
   UNUSED(path);
   const char * url = "http://monitor01.lab.it.uc3m.es/0012082/cgi-bin/gwebcache/gcache.cgi";
   return Conf::Conf("12", "sharepath", "incomingpath", "downloadpath", url);
}


Conf::Conf(std::string & localPort, std::string & sharePath,
           std::string & incomingPath, std::string & downloadPath,
           std::string & webCache) throw (std::invalid_argument, std::bad_alloc)
  : mSharePath(sharePath), mIncomingPath(incomingPath),
    mDownloadPath(downloadPath), mWebCache(webCache)
    // LocalPort and LocalPortNbo initilized bellow
{
   try {
      mLocalPort = string_to_int(localPort);
   } catch (std::invalid_argument & ia) {
      throw ;
   }
   mLocalPortNbo = htons(mLocalPort);
}

Conf::Conf(const char* localPort, const char* sharePath, const char* incomingPath,
           const char* downloadPath, const char* webCache) throw (std::invalid_argument, std::bad_alloc)
  : mSharePath(sharePath), mIncomingPath(incomingPath), mDownloadPath(downloadPath),
    mWebCache(webCache)
    // LocalPort and LocalPortNbo initilized bellow
{
   try {
      std::string s(localPort);
      mLocalPort = string_to_int(s);
   } catch (std::invalid_argument & ia) {
      throw ;
   }
   mLocalPortNbo = htons(mLocalPort);
}

std::ostream &
operator<<(std::ostream & os, Conf c) {
   os << "[Conf: localport=" << c.LocalPort()
      << ", sharepath=" << c.SharePath()
      << ", incomingpath=" << c.IncomingPath()
      << ", downloadpath=" << c.DownloadPath()
      << ", webcache=" << c.WebCache()
      << "]";
   return os;
}
