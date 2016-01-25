#include <stdio.h>
#include <stdlib.h>

#include <common/command.h>
#include <common/mode.h>

#include "args.h"
#include "serial.h"

int main(int argc, char** argv)
{
    Raspboot_Args args;
    Raspboot_Serial serial;

    if (raspbootArgsInit(&args) != 0 ||
        raspbootArgsParse(&args, argc, argv) < 0)
    {
        fprintf(stderr, "Unable to parse arguments.\n");
        return 1;
    }

    if (args.help)
    {
        raspbootArgsUsage();
        exit(0);
    }

    if (raspbootSerialInit(&serial, &args) != 0)
    {
        fprintf(stderr, "Unable to initialize serial connection\n");
        return 1;
    }

    printf("%c[2J%c[0;0H", 27, 27);

    unsigned char buffer[1024];
    FILE *ptr;
    ptr = fopen(args.binary,"rb");
    int count = fread(buffer, 1, sizeof(buffer), ptr);

    raspbootSerialPutW(&serial, args.location);
    raspbootSerialPutW(&serial, count);

    int i;
    for (i = 0; i < count; i++)
    {
        raspbootSerialPut(&serial, buffer[i]);
    }

    raspbootSerialFlush(&serial);

    raspbootArgsDestroy(&args);
    exit(0);
}
