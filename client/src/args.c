#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "args.h"

S32 raspbootArgsInit(Raspboot_Args* args)
{
    if (args == 0)
    {
        return -1;
    }

    args->go = 0;
    args->terminate = 0;
    args->help = 0;
    args->location = 0x10000;
    args->speed = 115200;

    args->binary = 0; 
    args->port = 0;

    return 0;
}

S32 raspbootArgsDestroy(Raspboot_Args* args)
{
    if (args == 0)
    {
        return -1;
    }

    if (args->binary)
    {
        free(args->binary);
    }

    if (args->port)
    {
        free(args->port);
    }

    return 0;
}

S32 raspbootArgsParse(Raspboot_Args* args, S32 argc, char** argv)
{
    if (args == 0)
    {
        return -1;
    }

    S32 c;
    while ((c = getopt(argc, argv, "ghtb:l:r:")) != -1)
    {
        switch (c)
        {
            case 'g':
                args->go = 1;
                break;

            case 'h':
                args->help = 1;
                break;

            case 't':
                args->terminate = 1;
                break;

            case 'b':
                args->binary = strdup(optarg);
                break;

            case 'l':
                args->location = atoi(optarg);
                break;

            case 'r':
                args->speed = atoi(optarg);
                break;

            case '?':
                if (optopt == 'b' || optopt == 'l' || optopt == 'r')
                {
                    fprintf(stderr,
                            "raspboot: Option `-%c` requires an argument.\n",
                            optopt);
                }

            default:
                return -1;
        }
    }

    if (optind >= argc)
    {
        if (args->help == 0)
        {
            fprintf(stderr, "raspboot: Missing serial port name.\n");
            return -1;
        }
        else
        {
            return 0;
        }
    }

    args->port = strdup(argv[optind]);
    args->help = 0;

    return 0;
}

void raspbootArgsUsage()
{
    printf("raspboot server 0.1\n"\
           "Usage: raspboot [options] port\n\n"\
           "Raspboot is a simple command line interface for interacting \n"\
           "with embedded systems over a serial connection. It can be \n"\
           "used to inspect memory state and upload binaries to a device \n"\
           "running the raspboot client.\n\n"\
           "Options:\n"\
           "  -b\tPath to the binary file to load.\n"\
           "  -g\tExecute the binary as soon as transfer is complete. By \n"\
           "\tdefault raspboot will enter interactive mode after the file \n"\
           "\ttransfer completes.\n"\
           "  -t\tTerminate Raspboot after transfer instead of entering\n"\
           "\titeractive mode. Usually used with -g\n"
           "  -h\tPrints this help message.\n"\
           "  -l\tThe memory location to write the binary to. Defaults to \n"\
           "\t0x10000. Raspboot will issue a warning if the address \n"\
           "\toverlaps with the location of the Raspboot client.\n"\
           "  -r\tThe serial connection baud rate. Defaults to 115200.\n"
           );
}