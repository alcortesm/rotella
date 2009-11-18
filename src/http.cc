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

   // build message, see http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html
   // example:
   // GET /cgi-bin/qwebcache/gcache.cgi?hostfile=1 HTTP/1.1\r\n
   // Host: monitor01.lab.it.uc3m.es\r\n
   // Conection: close\r\n
   // \r\n
   char * msg = NULL;
   size_t msg_sz = 0;
   {
      std::string s = "";
      s.append(MSG_HTTP_GET_METHOD);
      s.append(MSG_HTTP_SPACE);
      s.append(rUrl.Path());
      s.append(rUrl.Query());
      s.append(MSG_HTTP_SPACE);
      s.append(MSG_HTTP_VERSION);
      s.append(MSG_HTTP_EOL);
      s.append(MSG_HTTP_HOST_HDR_PREFIX);
      s.append(rUrl.Domain());
      s.append(MSG_HTTP_EOL);
      s.append(MSG_HTTP_CONNECTION_CLOSE_HDR);
      s.append(MSG_HTTP_EOL);

      msg = xstrdup(s.c_str());
      msg_sz = strlen(msg);
      debug << "Message:" << std::endl << s << std::endl ;
   }

   // send msg
   {
      ssize_t ns;
      ns = send(sock, msg, msg_sz, NO_SEND_FLAGS);
      if (-1 == ns) {
         close(sock);
         free(msg);
         throw_fname_errno("send");
      }
      debug << "Request sent (" << msg_sz << " bytes):" << std::endl << msg << std::endl;
      free(msg);
   }

   // try to read response
   char * buf;
   ssize_t nr;
   {
      size_t bufsz = 2048;
      buf = (char *) calloc(bufsz, sizeof(char));
      if (!buf) {
         close(sock);
         throw_fname_errno("calloc");
      }

      nr = recv(sock, buf, bufsz, NO_RECV_FLAGS);
      if (-1 == nr) {
         close(sock);
         free(buf);
         throw_fname_errno("recv");
      }
      if (0 == nr) throw_fname("recv", "Conection abnormally closed before reading response\n");

      // read 0 to detect connection closed by the server
      ssize_t last_nr;
      char last_buf[1];
      last_nr = recv(sock, last_buf, 1, NO_RECV_FLAGS);
      if (-1 == last_nr) {
         close(sock);
         free(buf);
         throw_fname_errno("recv");
      }
      if (0 != last_nr) throw_fname("recv", "Data received instead of conection closed by server\n");
      // nr == 0; connection closed by server
      debug << "Connection closed by server" << std::endl;
   }

   // close connection
   close (sock);

   // find the data section of the http response
   char * data;
   {
      data = strstr(buf, "\r\n\r\n");
      data += strlen("\r\n\r\n");
   }

   std::string * p_data = new std::string(buf, data-buf, nr);
   std::string * p_headers = new std::string(buf, 0, data-buf);
   free(buf);
   debug << "Received headers:\n" << * p_headers << std::endl ;
   debug << "Received data:\n" << * p_data << std::endl ;
   delete(p_headers);
   return p_data;
}
