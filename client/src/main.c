#include <stdio.h>
#include <stdlib.h>

#include <common/mode.h>

#include "args.h"
#include "output.h"
#include "serial.h"

int main(int argc, char** argv)
{
    Raspboot_Args args;
    Raspboot_Serial serial;

    if (raspbootArgsInit(&args) != 0 ||
        raspbootArgsParse(&args, argc, argv) < 0)
    {
        fprintf(stderr, "Unable to parse arguments.\n");
        exit(1);
    }

    if (args.help)
    {
        raspbootArgsUsage();
        exit(0);
    }

    if (raspbootSerialInit(&serial, &args) != 0)
    {
        fprintf(stderr, "Unable to initialize serial connection\n");
        exit(1);
    }

    if (args.binary != 0)
    {
        raspbootSerialPut(&serial, MODE_TRANSFER);
        raspbootOutputBinary(&serial, &args);
    }

    raspbootSerialPut(&serial, MODE_INTERACT);
    raspbootSerialFlush(&serial);

    raspbootArgsDestroy(&args);
    exit(0);
}
