#include "http.h"
#include "utils.h"
#include <cstring>
#include "DebugStream.h"
extern DebugStream debug;
#include <cstdlib>
#include "rotella.h"

inline void
add_to_msg(char* const msg, size_t* const poffset, const char* string)
{
   size_t sz = strlen(string);
   memcpy(msg+*poffset, string, sz);
   *poffset += sz;   
}

std::string *
http_get(const Url & rUrl) throw (std::runtime_error)
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
      if (sock == -1) throw_fname_errno("socket");
   }

   // connect to webcache
   {
      struct sockaddr server_sa = {0, {0}};
      socklen_t server_sa_len = sizeof(server_sa);
      {
         struct sockaddr_in server_in = {0, 0, {0}, {0}};
         server_in.sin_family = AF_INET;
         server_in.sin_port = rUrl.PortNbo();
         server_in.sin_addr = rUrl.Addr();
         memcpy((void *) &server_sa, (const void *) &server_in, sizeof(server_in));
      }

      int r;
      r = connect(sock, &server_sa, server_sa_len);
      if (r == -1) {
         close(sock);
         throw_fname_errno("connect");
      }
      debug << "Connected to " << rUrl.Canonical() << std::endl;
   }

   // send GET to obtain list of rotella nodes
   {
      char * msg;
      size_t msgsz = 1024;
      msg = (char *) calloc(msgsz, sizeof(char));
      if (!msg) {
         close(sock);
         throw_fname_errno("calloc");
      }
      size_t offset = 0;

      // GET /cgi-bin/qwebcache/gcache.cgi?hostfile=1 HTTP/1.0\r\n
      // \r\n
      add_to_msg(msg, &offset, MSG_HTTP_GET.c_str());
      add_to_msg(msg, &offset, MSG_HTTP_SPACE.c_str());
      add_to_msg(msg, &offset, rUrl.Path().c_str());
      add_to_msg(msg, &offset, rUrl.Query().c_str());
      add_to_msg(msg, &offset, MSG_HTTP_SPACE.c_str());
      add_to_msg(msg, &offset, MSG_HTTP_VERSION.c_str());
      add_to_msg(msg, &offset, MSG_HTTP_EOL.c_str());
      add_to_msg(msg, &offset, MSG_HTTP_EOL.c_str());

      ssize_t ns;
      ns = send(sock, msg, offset, NO_SEND_FLAGS);
      if (-1 == ns) {
         close(sock);
         free(msg);
         throw_fname_errno("send");
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
         close(sock);
         throw_fname_errno("calloc");
      }

      ssize_t nr;
      nr = recv(sock, msg, 2048, NO_RECV_FLAGS);
      if (-1 == nr) {
         close(sock);
         free(msg);
         throw_fname_errno("recv");
      }
      if (0 == nr) throw_fname("recv", "Conection abnormally closed before reading response\n");

      printf("Response received\n");

      // read 0 to detect connection closed by the server
      nr = recv(sock, msg, 2048, NO_RECV_FLAGS);
      if (-1 == nr) {
         close(sock);
         free(msg);
         throw_fname_errno("recv");
      }
      if (0 != nr) throw_fname("recv", "Data received instead of conection closed by server\n");
      // nr == 0; connection closed by server
      debug << "Connection closed by server" << std::endl;
   }

   // close connection
   {
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
