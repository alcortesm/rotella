#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cassert>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <cstring>

#include "utils.h"
#include "rotella.h"
#include "Command.h"
#include "Conf.h"
#include "DebugStream.h"

using std::string;
using std::endl;

Conf * conf_p;
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

// create the configuration variables with
// the values stored in the configuration
// file or defaults where available
int
digest_conf_file()
{
   const char * url = "beacon.numberzero.org/beacon/gwc.php";
   try {
      conf_p = new Conf("12", "sharepath", "incomingpath", "downloadpath", url);
   } catch (std::invalid_argument & ia) {
      std::cerr << ia.what() << endl;
      return -1;
   } catch (std::bad_alloc & ba) {
      std::cerr << ba.what() << ": out of memory" << endl;
      return -1;
   }
   return 0;
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
initialize(const Url & wc)
{
   UNUSED(wc);
   // create the socket
   int sock;
   {
      int tcp_proto;
      {
         // valgrind says that getprotobyname performs an invalid read:
         //   http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=543379
         // so until it is fixed I will use "6" as the tcp proto value
         //            struct protoent * pe_p;
         //            pe_p = my_getprotobyname("tcp");
         //            if (!pe_p) {
         //                perror("getprotobyname");
         //                exit(EXIT_FAILURE);
         //            }
         //            tcp_proto = pe_p->p_proto;
         tcp_proto = 6;
      }
      sock = socket(PF_INET, SOCK_STREAM, tcp_proto);
      if (sock == -1) {
         perror("socket");
         exit(EXIT_FAILURE);
      }
   }

   // connect to webcache
   //    {
   //        struct sockaddr server = {0, {0}};
   //        socklen_t serverlen = sizeof(server);
   //        {
   //            struct sockaddr_in server_in = {0, 0, {0}, {0}};
   //            server_in.sin_family = AF_INET;
   //            server_in.sin_port = wc.port_nbo();
   //            server_in.sin_addr = wc.addr();
   //            memcpy((void *) &server, (const void *) &server_in, sizeof(server_in));
   //        }
   //
   //        int r;
   //        r = connect(sock, &server, serverlen);
   //        if (r == -1) {
   //            perror("connect");
   //            exit(EXIT_FAILURE);
   //        }
   //    }

   // send GET to obtain list of rotella nodes
   //    {
   //        char * msg[100];
   //        int offset = 0;
   //
   //        memcpy(msg+offset, MSG_GET, strlen(MSG_GET));
   //        offset += strlen(MSG_GET);
   //
   //        memcpy(msg+offset, wc.path().c_str(), strlen(wc.path().c_str()));
   //        offset += strlen(wc.path().c_str());
   //
   //        memcpy(msg+offset, MSG_HTTPV, strlen(MSG_HTTPV));
   //        offset += strlen(MSG_HTTPV);
   //
   //        send(sock, msg, offset, NO_SEND_FLAGS);
   //    }
   // close the socket
   close(sock);
}

int
main(int argc, char ** argv)
{
   digest_args(argc, argv);

//    {
//       Url::test();
//       test_command();
//       exit(EXIT_SUCCESS);
//    }
    
   int r;
   r = digest_conf_file();
   if (r == -1)
      exit(EXIT_FAILURE);

   initialize(conf_p->webcache());
   //    inf_loop();
   delete conf_p;
   exit(EXIT_SUCCESS);
}
