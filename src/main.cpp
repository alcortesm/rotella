#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "utils.h"
#include "rotella.h"

using std::string;
using std::endl;

struct conf conf;
Debug_stream debug(std::cout, DEF_DEBUG);

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
void
digest_conf_file()
{
    conf.localport = DEFAULT_PORT;
    conf.sharepath = string("Sharepath");
    conf.incomingpath = string("Incomingpath");
    conf.downloadpath = string("Downloadpath");
    conf.rowebcache = string("Rowebcache");
    debug << conf.localport << endl;
    debug << conf.sharepath << endl;
    debug << conf.incomingpath << endl;
    debug << conf.downloadpath << endl;
    debug << conf.rowebcache << endl;
    return;
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

int
main(int argc, char ** argv)
{
    digest_args(argc, argv);
    digest_conf_file();
//    inf_loop();
    exit(EXIT_SUCCESS);
}
