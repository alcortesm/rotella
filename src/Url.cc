#include "Url.h"
#include "utils.h"
#include "DebugStream.h"
extern DebugStream debug;

#include <sstream>
#include <cstring>
#include <cstdlib>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cassert>
#include <iostream>

using std::string;
using std::endl;
using std::bad_alloc;
using std::invalid_argument;

void
Url::test(void)
{
   {
      const char * url = "hTTp://gwc.iblInx.coM:2108/gwc/cgi-BIn/fc";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(wcp->Proto() == "http");
      assert(wcp->Host()  == "gwc.iblinx.com");
      assert(wcp->Port()  == 2108);
      assert(wcp->Path()  == "/gwc/cgi-bin/fc");
      delete wcp;
   }
   {
      const char * url = "hTTp://gwc.iblinx.net:13/fc?bla.php#foobar";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(wcp->Proto() == "http");
      assert(wcp->Host()  == "gwc.iblinx.net");
      assert(wcp->Port()  == 13);
      assert(wcp->Path()  == "/fc");
      assert(wcp->Query()  == "bla.php");
      assert(wcp->Anchor()  == "foobar");
      delete wcp;
   }
   {
      const char * url = "http://gwc.iblinx.com/";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(wcp->Proto() == "http");
      assert(wcp->Host()  == "gwc.iblinx.com");
      assert(wcp->Port()  == 80);
      assert(wcp->Path()  == "");
      delete wcp;
   }
   {
      const char * url = "hTTp://gwc.iblInx.coM:2108";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(wcp->Proto() == "http");
      assert(wcp->Host()  == "gwc.iblinx.com");
      assert(wcp->Port()  == 2108);
      assert(wcp->Path()  == "");
      delete wcp;
   }
   {
      const char * url = "hTTp://gwc.iblInx.coM:2108#bla";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(wcp->Proto() == "http");
      assert(wcp->Host()  == "gwc.iblinx.com");
      assert(wcp->Port()  == 2108);
      assert(wcp->Path()  == "");
      assert(wcp->Anchor() == "bla");
      delete wcp;
   }
   {
      const char * url = "hTTp://a";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(wcp->Proto() == "http");
      assert(wcp->Host()  == "a");
      assert(wcp->Port()  == 80);
      assert(wcp->Path()  == "");
      delete wcp;
   }
   {
      const char * url = "hTTp://a/bla/foo.sh";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(wcp->Proto() == "http");
      assert(wcp->Host()  == "a");
      assert(wcp->Port()  == 80);
      assert(wcp->Path()  == "/bla/foo.sh");
      delete wcp;
   }
   {
      const char * url = "hTTp://a:1234";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(wcp->Proto() == "http");
      assert(wcp->Host()  == "a");
      assert(wcp->Port()  == 1234);
      assert(wcp->Path()  == "");
      delete wcp;
   }
   {
      const char * url = "htatp://www.google.com:120/bla/foo.sh";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         goto ok1;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      std::cerr << "An exception was not thrown (bad proto)" << endl;
      exit(EXIT_FAILURE);
   ok1:;
   }
   {
      const char * url = "ahttp://www.google.com:120/bla/foo.sh";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         goto ok2;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      std::cerr << "An exception was not thrown (prefix at proto)" << endl;
      exit(EXIT_FAILURE);
   ok2:;
   }
   {
      const char * url = "http:/a/www.google.com:120/bla/foo.sh";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         goto ok3;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      std::cerr << "An exception was not thrown (bad colon_slash_slash)" << endl;
      exit(EXIT_FAILURE);
   ok3:;
   }
   {
      const char * url = "http://:120/bla/foo.sh";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         goto ok4;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      std::cerr << "An exception was not thrown (no host name with port)" << endl;
      exit(EXIT_FAILURE);
   ok4:;
   }
   {
      const char * url = "http://www.google.com:12a0/bla/foo.sh";
      Url * wcp;
      try {
         wcp = new Url(url);
      } catch (Url::MalformedUrlException & mue) {
         goto ok5;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      std::cerr << "An exception was not thrown (bad port)" << endl;
      exit(EXIT_FAILURE);
   ok5:;
      {
         const char * url = "http:///";
         Url * wcp;
         try {
            wcp = new Url(url);
         } catch (Url::MalformedUrlException & mue) {
            goto ok6;
         } catch (std::bad_alloc & ba) {
            std::cerr << ba.what() << ": out of memory" << endl;
            goto error;
         }
         std::cerr << "An exception was not thrown (no host, no port, no path)" << endl;
         exit(EXIT_FAILURE);
      ok6:;
      }
      {
         const char * url = "http://:32";
         Url * wcp;
         try {
            wcp = new Url(url);
         } catch (Url::MalformedUrlException & mue) {
            goto ok7;
         } catch (std::bad_alloc & ba) {
            std::cerr << ba.what() << ": out of memory" << endl;
            goto error;
         }
         std::cerr << "An exception was not thrown (no host, no path)" << endl;
         exit(EXIT_FAILURE);
      ok7:;
      }
      {
         const char * url = "http://www.google.com:-32/bla/foo.sh";
         Url * wcp;
         try {
            wcp = new Url(url);
         } catch (Url::MalformedUrlException & mue) {
            goto ok8;
         } catch (std::bad_alloc & ba) {
            std::cerr << ba.what() << ": out of memory" << endl;
            goto error;
         }
         std::cerr << "An exception was not thrown (negative port number)" << endl;
         exit(EXIT_FAILURE);
      ok8:;
      }
      {
         const char * url = "http://www.google.com:999999/bla/foo.sh";
         Url * wcp;
         try {
            wcp = new Url(url);
         } catch (Url::MalformedUrlException & mue) {
            goto ok9;
         } catch (std::bad_alloc & ba) {
            std::cerr << ba.what() << ": out of memory" << endl;
            goto error;
         }
         std::cerr << "An exception was not thrown (port number too big)" << endl;
         exit(EXIT_FAILURE);
      ok9:;
      }
   }
   {
      const char * proto = "http";
      const char * host = "it.uc3m.es";
      uint16_t port = 8080;
      const char * path = "/alcortes/index.html";
      const char * query = "test.php";
      const char * anchor = "bla";

      try {
         Url url = Url::FromParts(proto, host, path, port, query, anchor);
         assert(url.Proto() == proto);
         assert(url.Host()  == host);
         assert(url.Port()  == port);
         assert(url.Path()  == path);
         assert(url.Query()  == query);
         assert(url.Anchor()  == anchor);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
   }
   return;
 error:
   exit(EXIT_FAILURE);
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
parse_url(const string & url, string & proto, string & host, uint16_t & port, string & path, string & query, string & anchor)
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

   bool has_query;
   char * mQuerystart;
   const char * mQueryend;
   size_t mQuerysz;

   bool has_anchor;
   char * mAnchorstart;
   const char * mAnchorend;
   size_t mAnchorsz;

   // c_str copy to play with
   start = strdup(url.c_str());
   if (!start)
      throw bad_alloc(); 

   // simplify comparisons with to_lower
   to_lower(start);

   // add http:// at the beginning if it's not already there
   if (! has_colon_slash_slash(start))
      prefix_http_colon_slash_slash(&start);
   sz = strlen(start);
   end = start + sz;
   //    debug << "start=" << start << endl;

   // find if there is an anchor strip it
   // find anchor end
   mAnchorend = end;
   // find anchor start
   mAnchorstart = strstr(start, URL_ANCHOR_SEPARATOR);
   if (mAnchorstart == end-1) { // empty anchor or last character is '#'
      free(start);
      throw Url::MalformedUrlException(url + ": empty anchor");
   }
   if (mAnchorstart == NULL) { // no anchor
      has_anchor = false;
   } else {
      has_anchor = true;
      mAnchorstart++; // advance the URL_ANCHOR_SEPARATOR
      mAnchorsz = mAnchorend - mAnchorstart;
      anchor = string(mAnchorstart, mAnchorsz);
      mAnchorstart--; // go back to URL_ANCHOR_SEPARATOR
      *mAnchorstart = '\0';
      sz = strlen(start);
      end = start + sz;
   }

   // find if there is a query and strip it
   // find query end
   mQueryend = end;
   // find anchor start
   mQuerystart = strstr(start, URL_QUERY_SEPARATOR);
   if (mQuerystart == end-1) { // empty query or last character is '#', strip it
      free(start);
      throw Url::MalformedUrlException(url + ": empty query");
   }
   if (mQuerystart == NULL) { // no query
      has_query = false;
   } else { // there is a query, store it and strip it
      has_query = true;
      mQuerystart++; // advance the URL_QUERY_SEPARATOR
      mQuerysz = mQueryend - mQuerystart;
      query = string(mQuerystart, mQuerysz);
      mQuerystart--; // go back to URL_ANCHOR_SEPARATOR
      *mQuerystart = '\0';
      sz = strlen(start);
      end = start + sz;
   }

   // find proto start
   mProtostart = strstr(start, HTTP_PROTO);
   if (mProtostart != start) {
      free(start);
      throw Url::MalformedUrlException(url + ": don't start with \"http\"");
   }
   // find proto end
   mProtoend = strstr(mProtostart, COLON_SLASH_SLASH);
   if (mProtoend != mProtostart + strlen(HTTP_PROTO)) {
      free(start);
      throw Url::MalformedUrlException(url + ": \"://\" not found after protocol");
   }
   // get proto
   mProtosz = mProtoend - mProtostart;
   proto = string(mProtostart, mProtosz);
   //    debug << "proto=" << proto << endl;


   // find host start
   mHoststart = mProtoend + strlen(COLON_SLASH_SLASH);



   // find path end
   mPathend = end;
   // find path start
   mPathstart = strstr(mHoststart, URL_PATH_SEPARATOR);
   if (!mPathstart)
      mPathstart = mPathend;
   // get path
   mPathsz = mPathend - mPathstart;
   if (mPathsz == 1)
      mPathsz = 0; // if path is "/" there is no path
   path = string(mPathstart, mPathsz);
   //    debug << "path="  << path  << endl; 

   // find port end
   port_end = mPathstart;
   // find port start
   port_start = strstr(mHoststart, URL_PORT_SEPARATOR);
   // get port
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

   // find host end
   mHostend = port_start;
   // get host
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

Url::Url(const string & rUrl) throw (Url::MalformedUrlException, bad_alloc)
   : mIp("")
{
   mAddr.s_addr=0;
   try {
      parse_url(rUrl, mProto, mHost, mPort, mPath, mQuery, mAnchor);
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
   if (HasQuery()) {
      mCanonical.append(URL_QUERY_SEPARATOR);
      mCanonical.append(mQuery);
   }
   if (HasAnchor()) {
      mCanonical.append(URL_ANCHOR_SEPARATOR);
      mCanonical.append(mAnchor);
   }

   mPortNbo = htons(mPort);
   debug << *this << endl;
}


Url
Url::FromParts(const std::string & rProto,
       const std::string & rHost,
       const std::string & rPath,
       const uint16_t      port,
       const std::string & rQuery,
       const std::string & rAnchor) throw (Url::MalformedUrlException, std::bad_alloc)
{
   string canonical = std::string("");
   canonical.append(rProto);
   canonical.append(COLON_SLASH_SLASH);
   canonical.append(rHost);
   canonical.append(URL_PORT_SEPARATOR);
   canonical.append(uint16_to_string(port));
   canonical.append(rPath);
   if (! rQuery.empty()) {
      canonical.append(URL_QUERY_SEPARATOR);
      canonical.append(rQuery);
   }
   if (! rAnchor.empty()) {
      canonical.append(URL_ANCHOR_SEPARATOR);
      canonical.append(rAnchor);
   }
   Url url(canonical);
   return url;
}

std::ostream &
operator<<(std::ostream & os, Url wc) {
   os << "[Url " << wc.Canonical() << "]";
   return os;
}
