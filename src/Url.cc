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

/* string */ const string Url::DOMAIN_SEPARATOR = string("://");
/* string */ const string Url::PORT_SEPARATOR = string(":");
/* string */ const string Url::PATH_SEPARATOR = string("/");
/* string */ const string Url::QUERY_SEPARATOR = string("?");
/* string */ const string Url::ANCHOR_SEPARATOR = string("#");
/* string */ const string Url::PROTO_HTTP = string("http");
/* string */ const uint16_t Url::HTTP_DEFAULT_PORT = 80;

/* static */ void
Url::test(void)
{
   // test the private constructor
   // this test involves a DNS resolution, you'll need a working resolver
   // of it.uc3m.es
   {
      const char * proto = "http";
      const char * domain = "it.uc3m.es";
      const char * ip = "163.117.139.31";
      uint16_t port = 8080;
      const char * path = "alcortes/index.html";
      const char * query = "test.php?other=yes";
      const char * anchor = "bla";
      const char * canonical = "http://it.uc3m.es:8080/alcortes/index.html?test.php?other=yes#bla";

      try {
         Url url = Url(proto, port, domain, path, query, anchor);
         assert(url.Proto() == proto);
         assert(url.Port() == port);
         assert(url.PortNbo() == htons(port));
         assert(url.Domain() == domain);
         struct in_addr ia;
         {
            int r = inet_aton(ip, &ia);
            if (r == 0) {
               std::cerr << "Provided an invalid ip: %s\n" << endl;
               goto error;
            }
         }
         assert(memcmp(&url.Addr(), &ia, sizeof(ia)) == 0);
         assert(url.Ip() == "163.117.139.31");
         assert(url.Path() == path);
         assert(url.Query() == query);
         assert(url.Anchor() == anchor);
         assert(url.Canonical() == canonical);
         assert(url.HasPath() == true);
         assert(url.HasQuery() == true);
         assert(url.HasAnchor() == true);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      } catch (Url::NameResolutionException & nre) {
         std::cerr << nre.what() << endl;
         goto error;
      } catch (Url::NetworkException & ne) {
         std::cerr << ne.what() << endl;
         goto error;
      }
   }
   // more tests on the private constructor
   // no more DNS resolutions from now on
   // no anchor
   {
      const char * proto = "http";
      const char * domain = "it.uc3m.es";
      uint16_t port = 8080;
      const char * path = "alcortes/index.html";
      const char * query = "test.php?other=yes";
      const char * anchor = "";
      const char * canonical = "http://it.uc3m.es:8080/alcortes/index.html?test.php?other=yes";

      try {
         Url url = Url(proto, port, domain, path, query, anchor);
         assert(url.Proto() == proto);
         assert(url.Port() == port);
         assert(url.PortNbo() == htons(port));
         assert(url.Domain() == domain);
         assert(url.Path() == path);
         assert(url.Query() == query);
         assert(url.Anchor() == anchor);
         assert(url.Canonical() == canonical);
         assert(url.HasPath() == true);
         assert(url.HasQuery() == true);
         assert(url.HasAnchor() == false);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
   }
   // no query
   {
      const char * proto = "http";
      const char * domain = "it.uc3m.es";
      uint16_t port = 8080;
      const char * path = "alcortes/index.html";
      const char * query = "";
      const char * anchor = "bla";
      const char * canonical = "http://it.uc3m.es:8080/alcortes/index.html#bla";

      try {
         Url url = Url(proto, port, domain, path, query, anchor);
         assert(url.Proto() == proto);
         assert(url.Port() == port);
         assert(url.PortNbo() == htons(port));
         assert(url.Domain() == domain);
         assert(url.Path() == path);
         assert(url.Query() == query);
         assert(url.Anchor() == anchor);
         assert(url.Canonical() == canonical);
         assert(url.HasPath() == true);
         assert(url.HasQuery() == false);
         assert(url.HasAnchor() == true);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
   }
   // with separators
   {
      const char * proto = "http";
      const char * domain = "it.uc3m.es";
      uint16_t port = 8080;
      const char * path = "/alcortes/index.html";
      const char * query = "?name=yes?user=no";
      const char * anchor = "#bla";
      const char * canonical = "http://it.uc3m.es:8080/alcortes/index.html?name=yes?user=no#bla";

      try {
         Url url = Url(proto, port, domain, path, query, anchor);
         assert(url.Proto() == proto);
         assert(url.Port() == port);
         assert(url.PortNbo() == htons(port));
         assert(url.Domain() == domain);
         assert(url.Path() == path);
         assert(url.Query() == query);
         assert(url.Anchor() == anchor);
         assert(url.Canonical() == canonical);
         assert(url.HasPath() == true);
         assert(url.HasQuery() == true);
         assert(url.HasAnchor() == true);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
   }
   {
      const char * url_txt = "hTTp://gwc.iblInx.coM:2108/gwc/cgi-BIn/fc";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "gwc.iblinx.com");
      assert(p_url->Port()  == 2108);
      assert(p_url->Path()  == "/gwc/cgi-bin/fc");
      delete(p_url);
   }
   {
      const char * url_txt = "hTTp://gwc.iblinx.net:13/fc?bla.php#foobar";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "gwc.iblinx.net");
      assert(p_url->Port()  == 13);
      assert(p_url->Path()  == "/fc");
      assert(p_url->Query()  == "bla.php");
      assert(p_url->Anchor()  == "foobar");
      delete p_url;
   }
   {
      const char * url_txt = "http://gwc.iblinx.com/";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "gwc.iblinx.com");
      assert(p_url->Port()  == 80);
      assert(p_url->Path()  == "");
      delete p_url;
   }
   {
      const char * url_txt = "hTTp://gwc.iblInx.coM:2108";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "gwc.iblinx.com");
      assert(p_url->Port()  == 2108);
      assert(p_url->Path()  == "");
      delete p_url;
   }
   {
      const char * url_txt = "hTTp://gwc.iblInx.coM:2108#bla";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "gwc.iblinx.com");
      assert(p_url->Port()  == 2108);
      assert(p_url->Path()  == "");
      assert(p_url->Anchor() == "bla");
      delete p_url;
   }
   {
      const char * url_txt = "hTTp://a";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "a");
      assert(p_url->Port()  == 80);
      assert(p_url->Path()  == "");
      delete p_url;
   }
   {
      const char * url_txt = "hTTp://a/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "a");
      assert(p_url->Port()  == 80);
      assert(p_url->Path()  == "/bla/foo.sh");
      delete p_url;
   }
   {
      const char * url_txt = "hTTp://a:1234";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         std::cerr << mue.what() << endl;
         goto error;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      assert(p_url->Proto() == "http");
      assert(p_url->Domain()  == "a");
      assert(p_url->Port()  == 1234);
      assert(p_url->Path()  == "");
      delete p_url;
   }
   {
      const char * url_txt = "htatp://www.google.com:120/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
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
      const char * url_txt = "ahttp://www.google.com:120/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
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
      const char * url_txt = "http:/a/www.google.com:120/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
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
      const char * url_txt = "http://:120/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
      } catch (Url::MalformedUrlException & mue) {
         goto ok4;
      } catch (std::bad_alloc & ba) {
         std::cerr << ba.what() << ": out of memory" << endl;
         goto error;
      }
      std::cerr << "An exception was not thrown (no domain name with port)" << endl;
      exit(EXIT_FAILURE);
   ok4:;
   }
   {
      const char * url_txt = "http://www.google.com:12a0/bla/foo.sh";
      Url * p_url;
      try {
         p_url = new Url(url_txt);
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
         const char * url_txt = "http:///";
         Url * p_url;
         try {
            p_url = new Url(url_txt);
         } catch (Url::MalformedUrlException & mue) {
            goto ok6;
         } catch (std::bad_alloc & ba) {
            std::cerr << ba.what() << ": out of memory" << endl;
            goto error;
         }
         std::cerr << "An exception was not thrown (no domain, no port, no path)" << endl;
         exit(EXIT_FAILURE);
      ok6:;
      }
      {
         const char * url_txt = "http://:32";
         Url * p_url;
         try {
            p_url = new Url(url_txt);
         } catch (Url::MalformedUrlException & mue) {
            goto ok7;
         } catch (std::bad_alloc & ba) {
            std::cerr << ba.what() << ": out of memory" << endl;
            goto error;
         }
         std::cerr << "An exception was not thrown (no domain, no path)" << endl;
         exit(EXIT_FAILURE);
      ok7:;
      }
      {
         const char * url_txt = "http://www.google.com:-32/bla/foo.sh";
         Url * p_url;
         try {
            p_url = new Url(url_txt);
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
         const char * url_txt = "http://www.google.com:999999/bla/foo.sh";
         Url * p_url;
         try {
            p_url = new Url(url_txt);
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
   return;
 error:
   exit(EXIT_FAILURE);
}

bool
has_colon_slash_slash(const char * s) {
   return (strstr(s, Url::DOMAIN_SEPARATOR.c_str()) != NULL);
}

void
prefix_http_colon_slash_slash(char* * sp) throw (bad_alloc) {
   char * news = (char *) calloc(sizeof(char), strlen(*sp)
                                 + Url::PROTO_HTTP.size()
                                 + Url::DOMAIN_SEPARATOR.size() + 1);
   if (!news)
      throw std::bad_alloc();
   strcat(news, Url::PROTO_HTTP.c_str());
   strcat(news, Url::DOMAIN_SEPARATOR.c_str());
   strcat(news, *sp);
   free(*sp);
   *sp = news;
}

// TODO: only proto & domain must be case insensitive
void
parse_url(const string & url, string & proto, uint16_t & port, string & domain,  string & path, string & query, string & anchor)
   throw (Url::MalformedUrlException, bad_alloc) {
   char * start;
   const char * end;
   size_t sz;

   const char * p_proto_start;
   const char * p_proto_end;
   size_t proto_sz;
    
   const char * p_domain_start;
   const char * p_domain_end;
   size_t domain_sz;
    
   const char * p_port_start;
   const char * p_port_end;
   size_t port_sz;
    
   const char * p_path_start;
   const char * p_path_end;
   size_t path_sz;

   bool has_query;
   char * p_query_start;
   const char * p_query_end;
   size_t query_sz;

   bool has_anchor;
   char * p_anchor_start;
   const char * p_anchor_end;
   size_t anchor_sz;

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
   p_anchor_end = end;
   // find anchor start
   p_anchor_start = strstr(start, Url::ANCHOR_SEPARATOR.c_str());
   if (p_anchor_start == end-1) { // empty anchor or last character is '#'
      free(start);
      throw Url::MalformedUrlException(url + ": empty anchor");
   }
   if (p_anchor_start == NULL) { // no anchor
      has_anchor = false;
   } else {
      has_anchor = true;
      p_anchor_start++; // advance the URL_ANCHOR_SEPARATOR
      anchor_sz = p_anchor_end - p_anchor_start;
      anchor = string(p_anchor_start, anchor_sz);
      p_anchor_start--; // go back to URL_ANCHOR_SEPARATOR
      *p_anchor_start = '\0';
      sz = strlen(start);
      end = start + sz;
   }

   // find if there is a query and strip it
   // find query end
   p_query_end = end;
   // find anchor start
   p_query_start = strstr(start, Url::QUERY_SEPARATOR.c_str());
   if (p_query_start == end-1) { // empty query or last character is '#', strip it
      free(start);
      throw Url::MalformedUrlException(url + ": empty query");
   }
   if (p_query_start == NULL) { // no query
      has_query = false;
   } else { // there is a query, store it and strip it
      has_query = true;
      p_query_start++; // advance the URL_QUERY_SEPARATOR
      query_sz = p_query_end - p_query_start;
      query = string(p_query_start, query_sz);
      p_query_start--; // go back to URL_ANCHOR_SEPARATOR
      *p_query_start = '\0';
      sz = strlen(start);
      end = start + sz;
   }

   // find proto start
   p_proto_start = strstr(start, Url::PROTO_HTTP.c_str());
   if (p_proto_start != start) {
      free(start);
      throw Url::MalformedUrlException(url + ": don't start with \"http\"");
   }
   // find proto end
   p_proto_end = strstr(p_proto_start, Url::DOMAIN_SEPARATOR.c_str());
   if (p_proto_end != p_proto_start + Url::PROTO_HTTP.size()) {
      free(start);
      throw Url::MalformedUrlException(url + ": \"://\" not found after protocol");
   }
   // get proto
   proto_sz = p_proto_end - p_proto_start;
   proto = string(p_proto_start, proto_sz);
   //    debug << "proto=" << proto << endl;


   // find domain start
   p_domain_start = p_proto_end + Url::DOMAIN_SEPARATOR.size();



   // find path end
   p_path_end = end;
   // find path start
   p_path_start = strstr(p_domain_start, Url::PATH_SEPARATOR.c_str());
   if (!p_path_start)
      p_path_start = p_path_end;
   // get path
   path_sz = p_path_end - p_path_start;
   if (path_sz == 1)
      path_sz = 0; // if path is "/" there is no path
   path = string(p_path_start, path_sz);
   //    debug << "path="  << path  << endl; 

   // find port end
   p_port_end = p_path_start;
   // find port start
   p_port_start = strstr(p_domain_start, Url::PORT_SEPARATOR.c_str());
   // get port
   if (!p_port_start) {
      p_port_start = p_port_end;
      port = Url::HTTP_DEFAULT_PORT;
   } else {
      port_sz = p_port_end - p_port_start;
      string ports;
      ports = string(p_port_start+1, port_sz-1); // we don't want the ':' to be part of the port
      try {
         int tmp = string_to_int(ports);
         port = int_to_uint16(tmp);
      } catch (invalid_argument & ia) {
         free(start);
         throw Url::MalformedUrlException();
      }
   }
   //    debug << "port="  << port  << endl;

   // find domain end
   p_domain_end = p_port_start;
   // get domain
   domain_sz = p_domain_end - p_domain_start;
   if (domain_sz == 0) {
      free(start);
      throw Url::MalformedUrlException(url + ": domain name not found");
   }
   domain = string(p_domain_start, domain_sz);
   //    debug << "domain="  << domain  << endl;
   
   free(start);
   return;
}

void
Url::get_ip_and_addr() const throw (Url::NameResolutionException, Url::NetworkException) {
   struct hostent * he;
   he = gethostbyname(mDomain.c_str());
   if (!he) {
      switch (h_errno) {
      case HOST_NOT_FOUND : throw Url::NameResolutionException(mDomain + ": domain not found");
      case NO_DATA : throw Url::NameResolutionException(mDomain + ": no IP for this domain");
      case NO_RECOVERY : throw Url::NameResolutionException("name server error while recovering " + mDomain);
      case TRY_AGAIN : throw Url::NameResolutionException("temporary error of name server while recovering " + mDomain + ", try again later");
      default : throw Url::NameResolutionException("unknown error with getdomainbyname()");
      }
   }
   memcpy(&mAddr, &he->h_addr, sizeof(he->h_addr));
   mIp = string(inet_ntoa(mAddr));
    
   struct addrinfo * ai;
   int error;
   /* resolve the domain name into a list of addresses */
   error = getaddrinfo(mDomain.c_str(), NULL, NULL, &ai);
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

// /* static */ Url *
// Url::CreateFromTxt(const std::string & rTxt)
//    throw (Url::MalformedUrlException, std::bad_alloc)
// {
//    string proto;
//    uint16_t port;
//    string domain;
//    string path;
//    string query;
//    string anchor;
//    parse_url(rTxt, proto, port, domain, path, query, anchor);
//    return new Url(proto, port, domain, path, query, anchor);
// }

void
Url::Init(const string & rProto,
     uint16_t port,
     const string & rDomain,
     const string & rPath,
     const string & rQuery,
     const string & rAnchor)
{
   mProto = rProto;
   mPort = port;
   mDomain = rDomain;
   mPath = rPath;
   mQuery = rQuery;
   mAnchor = rAnchor;

   mPortNbo = htons(mPort);
   mIp = string("");
   mAddr.s_addr = 0;
   mCanonical = string();
   mCanonical.append(mProto);
   mCanonical.append(Url::DOMAIN_SEPARATOR);
   mCanonical.append(mDomain);
   mCanonical.append(Url::PORT_SEPARATOR);
   mCanonical.append(uint16_to_string(mPort));
   if (HasPath()) {
      if (! begins_with(mPath, Url::PATH_SEPARATOR))
         mCanonical.append(Url::PATH_SEPARATOR);
      mCanonical.append(mPath);
   }
   if (HasQuery()) {
      if (! begins_with(mQuery, Url::QUERY_SEPARATOR))
         mCanonical.append(Url::QUERY_SEPARATOR);
      mCanonical.append(mQuery);
   }
   if (HasAnchor()) {
      if (! begins_with(mAnchor, Url::ANCHOR_SEPARATOR))
         mCanonical.append(Url::ANCHOR_SEPARATOR);
      mCanonical.append(mAnchor);
   }
}

Url::Url(const std::string & rProto,
         const uint16_t      port,
         const std::string & rDomain,
         const std::string & rPath,
         const std::string & rQuery,
         const std::string & rAnchor)
   throw (Url::MalformedUrlException, std::bad_alloc)
{
   Url::Init(rProto, port, rDomain, rPath, rQuery, rAnchor);
   debug << *this << endl ;
}

Url::Url(const std::string & rTxt)
   throw (Url::MalformedUrlException, std::bad_alloc)
{
   string proto;
   uint16_t port;
   string domain;
   string path;
   string query;
   string anchor;
   parse_url(rTxt, proto, port, domain, path, query, anchor);

   Url::Init(proto, port, domain, path, query, anchor);
   debug << *this << endl ;
}

/* static */ Url
Url::AddQuery(const Url & rUrl, const std::string & rQuery)
   throw (Url::MalformedUrlException, std::bad_alloc)
{
   const string & proto = rUrl.Proto();
   uint16_t port = rUrl.Port();
   const string & domain = rUrl.Domain();
   const string & path = rUrl.Path();
   const string & anchor = rUrl.Anchor();
   return Url(proto, port, domain, path, rQuery, anchor);   
}

std::ostream &
operator<<(std::ostream & os, Url wc) {
   os << "[Url " << wc.Canonical() << "]";
   return os;
}
