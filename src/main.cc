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
   const char * url = "http://monitor01.lab.it.uc3m.es/0012082/cgi-bin/gwebcache/gcache.cgi";
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


inline void
add_to_msg(char* const msg, size_t* const poffset, const char* string)
{
   size_t sz = strlen(string);
   memcpy(msg+*poffset, string, sz);
   *poffset += sz;   
}

// TODO add suport to throw exceptions
// TODO recv should be OK with fragmentation
//
// opens a socket, connect to the http server,
// sends an HTTP/1.0 request, read the response,
// wait for the server to close the remote socket,
// close the local socket and return the response
// as a string.
// If there is an error, throws an exception.
std::string *
http_get_response(const Url & rServer, const std::string & rQuery) throw (std::runtime_error)
{
   // open the socket
   int sock;
   {
      int tcp_proto;
      {
         // valgrind says that getprotobyname performs an invalid read:
         // http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=543379
         // so until it is fixed I will use "6" as the tcp proto value
         //   struct protoent * pe_p;
         // pe_p = my_getprotobyname("tcp");
         // if (!pe_p) {
         //   perror("getprotobyname");
         //   exit(EXIT_FAILURE);
         // }
         // tcp_proto = pe_p->p_proto;
         tcp_proto = 6;
      }
      sock = socket(PF_INET, SOCK_STREAM, tcp_proto);
      if (sock == -1)
         throw std::runtime_error(string("socket") + strerror(errno));
   }

   // connect to webcache
   {
      struct sockaddr server_sa = {0, {0}};
      socklen_t server_sa_len = sizeof(server_sa);
      {
         struct sockaddr_in server_in = {0, 0, {0}, {0}};
         server_in.sin_family = AF_INET;
         server_in.sin_port = rServer.PortNbo();
         server_in.sin_addr = rServer.Addr();
         memcpy((void *) &server_sa, (const void *) &server_in, sizeof(server_in));
      }

      int r;
      r = connect(sock, &server_sa, server_sa_len);
      if (r == -1) {
         close(sock);
         throw std::runtime_error(string("connect: ") + strerror(errno));
      }
      debug << "Connected to " << rServer.Canonical() << std::endl;
   }

   // send GET to obtain list of rotella nodes
   {
      char * msg;
      size_t msgsz = 1024;
      msg = (char *) calloc(msgsz, sizeof(char));
      if (!msg) {
         perror("calloc");
         exit(EXIT_FAILURE);
      }
      size_t offset = 0;

      // GET /cgi-bin/qwebcache/gcache.cgi?hostfile=1 HTTP/1.0\r\n
      // \r\n
      add_to_msg(msg, &offset, MSG_HTTP_GET.c_str());
      add_to_msg(msg, &offset, MSG_HTTP_SPACE.c_str());
      add_to_msg(msg, &offset, rServer.Path().c_str());
      add_to_msg(msg, &offset, rQuery.c_str());
      add_to_msg(msg, &offset, MSG_HTTP_SPACE.c_str());
      add_to_msg(msg, &offset, MSG_HTTP_VERSION.c_str());
      add_to_msg(msg, &offset, MSG_HTTP_EOL.c_str());
      add_to_msg(msg, &offset, MSG_HTTP_EOL.c_str());

      ssize_t ns;
      ns = send(sock, msg, offset, NO_SEND_FLAGS);
      if (-1 == ns) {
         perror("send");
         exit(EXIT_FAILURE);
      }
      debug << "Request sent" << std::endl;
      free(msg);
   }
   // try to read response
   char * msg;
   {
      size_t msgsz = 2048;
      msg = (char *) calloc(msgsz, sizeof(char));
      if (!msg) {
         perror("calloc");
         exit(EXIT_FAILURE);
      }

      ssize_t nr;
      nr = recv(sock, msg, 2048, NO_RECV_FLAGS);
      if (-1 == nr) {
         perror("recv");
         exit(EXIT_FAILURE);
      }
      if (0 == nr) {
         fprintf(stderr, "Conection abnormally closed before reading response\n");
         exit(EXIT_FAILURE);
      }

      printf("Response received\n");

      // read 0 to detect connection closed by the server
      nr = recv(sock, msg, 2048, NO_RECV_FLAGS);
      if (-1 == nr) {
         perror("recv");
         exit(EXIT_FAILURE);
      }
      if (0 != nr) {
         fprintf(stderr, "Data received instead of conection closed by server\n");
         exit(EXIT_FAILURE);
      }
      // nr == 0; connection closed by server

      debug << "Connection closed by server" << std::endl;
      close (sock);
      debug << "Connection closed locally" << std::endl;
   }

   // find the data section of the http response
   char * data;
   {
      data = strstr(msg, "\r\n\r\n");
      data += strlen("\r\n\r\n");
   }

   std::string * pResponse = new std::string(data);
   free(msg);
   return pResponse;
}

void
initialize(const Url & rWebCache)
{
   std::string* p_query;
   std::string* p_response;

   p_query = new string("?hostfile=1");
   debug << "Sending Hostfile request" << std::endl;
   try {
      p_response = http_get_response(rWebCache, *p_query);
   } catch (std::runtime_error & re) {
      delete p_query;
      throw re;
   }
   debug << "Hostfile query response: \"" << *p_response << "\"" << std::endl;
   delete(p_response);
   delete(p_query);

   p_query = new string("?ip=101.102.103.104:105");
   debug << "Sending Update request" << std::endl;
   try {
      p_response = http_get_response(rWebCache, *p_query);
   } catch (std::runtime_error & re) {
      delete p_query;
      throw re;      
   }
   debug << "Hostfile query response: \"" << *p_response << "\"" << std::endl;
   delete(p_response);
   delete(p_query);

   return;
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

   try {
      initialize(conf_p->webcache());
   } catch (std::runtime_error & re) {
      std::cerr << re.what() << endl;
      delete conf_p;
      exit(EXIT_FAILURE);
   }
   //    inf_loop();
   delete conf_p;
   exit(EXIT_SUCCESS);
}
