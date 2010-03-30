#include "http.h"
#include "utils.h"

void
http_test(void)
{
   {
       Url url = Url("http://www.google.com/index.html");
       std::string * p_response;
       p_response = http_get(url);
       delete(p_response);
   }

   {
       Url url = Url("http://monitor01.lab.it.uc3m.es/0012082/cgi-bin/gwebcache/gcache.cgi");
       std::string * p_response;
       p_response = http_get(url);
       delete(p_response);
   }

   {
       Url url = Url("http://monitor01.lab.it.uc3m.es/0012082/cgi-bin/gwebcache/gcache.cgi?hostfile=1");
       std::string * p_response;
       p_response = http_get(url);
       delete(p_response);
   }

   return;
}
