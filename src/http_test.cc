#include "http.h"
#include "utils.h"

void
http_test(void)
{
   Url url = Url("http://www.google.com/index.html");

   std::string * p_response;
   p_response = http_get(url);
   delete(p_response);

   return;
}
