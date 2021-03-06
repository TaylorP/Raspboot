#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "args.h"
#include "console.h"

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
    args->verbosity = CONSOLE_VERBOSITY_INFO;

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
    while ((c = getopt(argc, argv, "ghtb:l:r:v:")) != -1)
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
                args->location = (U32)strtol(optarg, NULL, 0);
                break;

            case 'r':
                args->speed = atoi(optarg);
                break;

            case 'v':
                args->verbosity = atoi(optarg);
                if (args->verbosity > 2)
                {
                    raspbootConsoleWarn(args,
                          "Invalid verbosity level `%d`. Using default of 2.",
                          args->verbosity);
                    args->verbosity = 2;
                }
                break;

            case '?':
                if (optopt == 'b' || optopt == 'l' || optopt == 'r')
                {
                    raspbootConsoleWarn(args,
                            "Option `-%c` requires an argument. Option ignored.\n",
                            optopt);
                }
                break;

            default:
                return -1;
        }
    }

    if (optind >= argc)
    {
        if (args->help == 0)
        {
            raspbootConsoleError(args, "Missing serial port name");
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

void raspbootArgsUsage(void)
{
    printf("raspboot client 0.1\n"\
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
           "\titeractive mode. Usually used with -g.\n"
           "  -h\tPrints this help message.\n"\
           "  -l\tThe memory location to write the binary to. Defaults to \n"\
           "\t0x10000. Raspboot will issue a warning if the address \n"\
           "\toverlaps with the location of the Raspboot server.\n"\
           "  -r\tThe serial connection baud rate. Must be a valid baud rate \n"
           "\tas specified in the termios(3) documentation, e.g. 300, 9600, \n"
           "\t57600, etc. Defaults to 115200.\n"\
           "  -v\tThe verbosity level for the client. Defaults to 2.\n"\
           "\t\t0 = errors only\n\t\t1 = warnings and errors\n"\
           "\t\t2 = all messages\n"
           );
}
