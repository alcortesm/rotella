#include<cstdlib>
#include<cstdio>
#include<ctime>

#include"utils.h"

void
usage(void)
{
    fprintf(stderr, "Usage:\n\trotella\n");
    exit(EXIT_FAILURE);
}

void
digest_args(int argc, char ** argv)
{
    UNUSED(argv);

    if (argc != 1)
        usage();
}

// create the configuration variables with
// the values stored in the configuration
// file or defaults where available
void
digest_conf_file()
{
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
    inf_loop();
    exit(EXIT_SUCCESS);
}
