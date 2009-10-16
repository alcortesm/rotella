#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cassert>
#include <stdexcept>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <cstring>
#include <error.h>
#include <errno.h>

#include "Url.h"
#include "utils.h"
#include "rotella.h"
#include "Command.h"
#include "Conf.h"
#include "DebugStream.h"
#include "http.h"
#include "prompt.h"

using std::string;
using std::endl;
using std::cerr;

DebugStream debug(std::cout, DEF_DEBUG);

void
usage(void)
{
   std::cerr << "Usage:\n\trotella [-d]" << endl;
   exit(EXIT_FAILURE);
}

void
digest_args(int argc, char ** argv)
{
   if (argc > 2)
      usage();
   if (argc == 2) {
      if (string("-d") == argv[1]) {
         debug.on() = true;
      } else {
         usage();
      }
   }
}

void
inf_loop(void)
{
   const struct timespec onesec = {1, 0};
   int r;

   while (true) {
      r = nanosleep(&onesec, NULL);
      if (r == -1) {
         perror("nanosleep");
         exit(EXIT_FAILURE);
      }
   }
}

void
test_command() {
   Command c("hola");
   debug << c << endl;
}

void
initialize(const Url & rWebCache) throw (std::runtime_error)
{
   debug << "** Initialization started" << endl;

   std::string* p_response;

   debug << "Sending Hostfile request..." << std::endl;
   try {
      Url hostfile_query = Url::AddQuery(rWebCache, "?hostfile=1");
      p_response = http_get(hostfile_query);
   } catch (std::runtime_error & re) {
      throw std::runtime_error(string("Initialization from webcache failed: ") + re.what());
   }
   debug << "Received Hostfile response: \"" << *p_response << "\"" << std::endl;
   delete(p_response);

   debug << "Sending Update request..." << std::endl;
   try {
      Url ip_query = Url::AddQuery(rWebCache, "?ip=101.102.103.104:105");
      p_response = http_get(ip_query);
   } catch (std::runtime_error & re) {
      throw re;      
   }
   debug << "Received update response: \"" << *p_response << "\"" << std::endl;
   delete(p_response);

   debug << "** Initialization finnished" << endl;
   return;
}

void
tests(void)
{
   debug << std::endl << std::endl << "Running unit tests!" <<  std::endl;
   Url::test();
   test_command();
   debug << "All tests passed!" <<  std::endl << std::endl << std::endl;
}

int
main(int argc, char ** argv)
{
   digest_args(argc, argv);

   tests();

   try {
      Conf conf = Conf::FromFile();
      initialize(conf.WebCache());
   } catch (std::exception & e) {
      cerr << e.what() << endl;
      goto error;
   }

   launch_prompt();

   exit(EXIT_SUCCESS);
 error:
   exit(EXIT_FAILURE);
}
