#include "Url.h"

// TODO recv should be OK with fragmentation
//
// opens a socket, connect to the http server,
// sends an HTTP/1.0 request, read the response,
// wait for the server to close the remote socket,
// close the local socket and return the response
// as a string.
// If there is an error, throws an exception.
std::string * http_get(const Url::Url & rUrl) throw (std::runtime_error);
void http_test(void);
