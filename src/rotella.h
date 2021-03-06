#ifndef ROTELLA_H
#define ROTELLA_H

#include <string>

// protocol constants
const int GUID_SZ           = 16;   /* bytes, rotella_protocol.txt:95 */
const int ASCII_CR          = 13;   /* rotella_protocol.txt:140 */
const int ASCII_LF          = 10;   /* rotella_protocol.txt:140 */
const int DEFAULT_PORT      = 6346; /* rotella_protocol:133 */
const int MAX_SERVENTS_CONNECTED = 3; /* rotella_protocol:223 */
const int MAX_HOPS          = 7;    /* rotella_protocol.txt:312 */
const int MAX_MESSAGE_SZ    = 4;    /* kB, rotella_protocol.txt:319 */
const int MAX_TTL           = 15;   /* rotella_protocol.txt:335 */
const int MAX_QUERY_SZ      = 256;  /* bytes, rotella_protocol.txt:344 */

// constant strings for messages
const std::string MSG_HTTP_GET_METHOD("GET");
const std::string MSG_HTTP_SPACE(" ");
const std::string MSG_HTTP_VERSION("HTTP/1.1");
const std::string MSG_HTTP_EOL("\r\n");
const std::string MSG_HTTP_HOST_HDR_PREFIX("Host: ");
const std::string MSG_HTTP_CONNECTION_CLOSE_HDR("Connection: close\r\n");
#define TCP_PROTO "tcp"

// mix constants
const bool DEF_DEBUG         = false;
const int NO_SEND_FLAGS      = 0;
const int NO_RECV_FLAGS      = 0;

#endif
