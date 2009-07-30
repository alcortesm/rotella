#include<cstdlib>
#include<cstdio>

#include"utils.h"

void
usage(void)
{
    fprintf(stderr, "Usage:\n\trotella\n");
    exit(EXIT_FAILURE);
}

void
check_args(int argc, char ** argv)
{
    UNUSED(argv);

    if (argc != 1)
        usage();
}

int
main(int argc, char ** argv)
{
    check_args(argc, argv);
    exit(EXIT_SUCCESS);
}
