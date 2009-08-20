#ifndef ROTELLA_H
#define ROTELLA_H

#include <string>

const bool DEF_DEBUG         = true;

const int GUID_SZ           = 16;   /* bytes, rotella_protocol.txt:95 */
const int ASCII_CR          = 13;   /* rotella_protocol.txt:140 */
const int ASCII_LF          = 10;   /* rotella_protocol.txt:140 */
const int DEFAULT_PORT      = 6346; /* rotella_protocol:133 */
const int MAX_SERVENTS_CONNECTED = 3; /* rotella_protocol:223 */
const int MAX_HOPS          = 7;    /* rotella_protocol.txt:312 */
const int MAX_MESSAGE_SZ    = 4;    /* kB, rotella_protocol.txt:319 */
const int MAX_TTL           = 15;   /* rotella_protocol.txt:335 */
const int MAX_QUERY_SZ      = 256;  /* bytes, rotella_protocol.txt:344 */

struct conf {
    int localport;
    std::string sharepath;
    std::string incomingpath;
    std::string downloadpath;
    std::string rowebcache;
};

#endif
