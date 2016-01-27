#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <common/command.h>
#include "output.h"

S32 raspbootOutputBinary(Raspboot_Serial* serial, Raspboot_Args* args)
{
    // Open the binary file for reading
    FILE *ptr = fopen(args->binary, "rb");
    if (ptr == 0)
    {
        fprintf(stderr,
                "raspboot: error opening %s: %s\n",
                args->binary, strerror(errno));
        return -1;
    }

    // Write the start transfer command byte
    raspbootSerialPut(serial, COMMAND_TRANSFER_INIT);

    // Write the memory address to load the binary
    raspbootSerialPutW(serial, args->location);

    // Find the file size and write it to the connection
    fseek(ptr, 0L, SEEK_END);
    raspbootSerialPutW(serial, ftell(ptr));
    fseek(ptr, 0L, SEEK_SET);
    
    // Write bytes in blocks of 1024
    U8 buffer[1024];
    int count, i;
    while (1)
    {
        count = fread(buffer, 1, sizeof(buffer), ptr);
        for (i = 0; i < count; i++)
        {
            raspbootSerialPut(serial, buffer[i]);
        }

        if (count < 1024)
        {
            break;
        }
    }

    // Write the end transfer command byte
    raspbootSerialPut(serial, COMMAND_TRANSFER_END);

    return 0;
}