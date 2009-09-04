#include "Url.h"
#include "utils.h"
#include "DebugStream.h"
extern DebugStream debug;

#include <sstream>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
extern int h_errno;
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::string;
using std::endl;
using std::bad_alloc;
using std::invalid_argument;

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
   throw (Url::MalformedUrlException, bad_alloc) {
   char * start;
   const char * end;
   size_t sz;

   const char * mProtostart;
   const char * mProtoend;
   size_t mProtosz;
    
   const char * mHoststart;
   const char * mHostend;
   size_t mHostsz;
    
   const char * port_start;
   const char * port_end;
   size_t port_sz;
    
   const char * mPathstart;
   const char * mPathend;
   size_t mPathsz;

   start = strdup(url.c_str());
   if (!start)
      throw bad_alloc(); 

   to_lower(start);
   if (! has_colon_slash_slash(start))
      prefix_http_colon_slash_slash(&start);
   sz = strlen(start);
   end = start + sz;
   //    debug << "start=" << start << endl;

   mProtostart = strstr(start, HTTP_PROTO);
   if (mProtostart != start) {
      free(start);
      throw Url::MalformedUrlException(url + ": don't start with \"http\"");
   }
   mProtoend = strstr(mProtostart, COLON_SLASH_SLASH);
   if (mProtoend != mProtostart + strlen(HTTP_PROTO)) {
      free(start);
      throw Url::MalformedUrlException(url + ": \"://\" not found after protocol");
   }
   mProtosz = mProtoend - mProtostart;
   proto = string(mProtostart, mProtosz);
   //    debug << "proto=" << proto << endl;

   mHoststart = mProtoend + strlen(COLON_SLASH_SLASH);
   mPathend = end;
   mPathstart = strstr(mHoststart, URL_PATH_SEPARATOR);
   if (!mPathstart)
      mPathstart = mPathend;
   mPathsz = mPathend - mPathstart;
   if (mPathsz == 1)
      mPathsz = 0; // if path is "/" there is no path
   path = string(mPathstart, mPathsz);
   //    debug << "path="  << path  << endl; 

   port_end = mPathstart;
   port_start = strstr(mHoststart, URL_PORT_SEPARATOR);
   if (!port_start) {
      port_start = port_end;
      port = (uint16_t) HTTP_DEFAULT_PORT;
   } else {
      port_sz = port_end - port_start;
      string ports;
      ports = string(port_start+1, port_sz-1); // we don't want the ':' to be part of the port
      try {
         int tmp = string_to_int(ports);
         port = int_to_uint16(tmp);
      } catch (invalid_argument & ia) {
         free(start);
         throw Url::MalformedUrlException();
      }
   }
   //    debug << "port="  << port  << endl;

   mHostend = port_start;
   mHostsz = mHostend - mHoststart;
   if (mHostsz == 0) {
      free(start);
      throw Url::MalformedUrlException(url + ": host name not found");
   }
   host = string(mHoststart, mHostsz);
   //    debug << "host="  << host  << endl;

   free(start);
   return;
}

void
Url::get_ip_and_addr() const throw (Url::NameResolutionException, Url::NetworkException) {
   struct hostent * he;
   he = gethostbyname(mHost.c_str());
   if (!he) {
      switch (h_errno) {
      case HOST_NOT_FOUND : throw Url::NameResolutionException(mHost + ": host not found");
      case NO_DATA : throw Url::NameResolutionException(mHost + ": no IP for this host");
      case NO_RECOVERY : throw Url::NameResolutionException("name server error while recovering " + mHost);
      case TRY_AGAIN : throw Url::NameResolutionException("temporary error of name server while recovering " + mHost + ", try again later");
      default : throw Url::NameResolutionException("unknown error with gethostbyname()");
      }
   }
   memcpy(&mAddr, &he->h_addr, sizeof(he->h_addr));
   mIp = string(inet_ntoa(mAddr));
    
   struct addrinfo * ai;
   int error;
   /* resolve the domain name into a list of addresses */
   error = getaddrinfo(mHost.c_str(), NULL, NULL, &ai);
   if (error != 0)
      throw Url::NetworkException(gai_strerror(error));

   struct addrinfo * curr_ai;
   struct sockaddr_in * sin;
   for (curr_ai=ai; curr_ai!=NULL; curr_ai=curr_ai->ai_next) {
      if (curr_ai->ai_addr->sa_family == AF_INET) {
         sin = (struct sockaddr_in *) curr_ai->ai_addr;
         memcpy(&mAddr, &sin->sin_addr, sizeof(sin->sin_addr));
         freeaddrinfo(ai);
         mIp = string(inet_ntoa(mAddr));
         return;
      }
   }
   freeaddrinfo(ai);
   throw Url::NetworkException("no ipv4 address found");
}

const std::string &
Url::Ip() const throw (Url::NameResolutionException, Url::NetworkException) {
   try {
      if (mIp == "")
         get_ip_and_addr();
   } catch (Url::NameResolutionException & nre) {
      throw;
   } catch (Url::NetworkException & ne) {
      throw;
   }
   return mIp;
}

const struct in_addr &
Url::Addr() const throw (Url::NameResolutionException, Url::NetworkException) {
   try {
      if (mIp == "")
         get_ip_and_addr();
   } catch (Url::NameResolutionException & nre) {
      throw;
   } catch (Url::NetworkException & ne) {
      throw;
   }
   return mAddr;
}

Url::Url(const string & url) throw (Url::MalformedUrlException, bad_alloc)
   : mIp("")
{
   mAddr.s_addr=0;
   try {
      parse_url(url, mProto, mHost, mPort, mPath);
   } catch (Url::MalformedUrlException & mue) {
      throw;
   } catch (bad_alloc & ba) {
      throw;
   }
   mCanonical = string();
   mCanonical.append(mProto);
   mCanonical.append(COLON_SLASH_SLASH);
   mCanonical.append(mHost);
   mCanonical.append(URL_PORT_SEPARATOR);
   mCanonical.append(uint16_to_string(mPort));
   mCanonical.append(mPath);

   mPortNbo = htons(mPort);
   debug << *this << endl;
}

std::ostream &
operator<<(std::ostream & os, Url wc) {
   os << "[Url " << wc.Canonical() << "]";
   return os;
}
