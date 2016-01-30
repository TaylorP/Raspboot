#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <common/command.h>
#include <common/mode.h>

#include "output.h"

S32 raspbootOutputMode(Raspboot_Serial* serial, const U8 mode)
{
    if (mode == MODE_TRANSFER)
    {
        printf("Entering transfer mode\n");
    }
    else if (mode == MODE_INTERACT)
    {
        printf("Entering interaction mode\n");
    }
    else
    {
        return -1;
    }

    return raspbootSerialPut(serial, mode);
}

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
    else
    {
        printf("\tOpened binary file %s\n", args->binary);
    }

    // Write the start transfer command byte
    raspbootSerialPut(serial, COMMAND_TRANSFER_INIT);

    // Write the memory address to load the binary
    raspbootSerialPutW(serial, args->location);

    // Find the file size and write it to the connection
    fseek(ptr, 0L, SEEK_END);
    U32 size = ftell(ptr);
    raspbootSerialPutW(serial, size);
    fseek(ptr, 0L, SEEK_SET);
    printf("\tWriting %d bytes of data to memory at address 0x%x\n",
           size,
           args->location);
    printf("\tUsing block size of %d bytes\n", OUTPUT_BLOCK_SIZE);

    // Write bytes in blocks of OUTPUT_BLOCK_SIZE (1024)
    U8 buffer[OUTPUT_BLOCK_SIZE];
    U32 count, i;
    U32 total = 0;
    U8 checksum = 0;

    while (1)
    {
        // Read from the binary file
        count = fread(buffer, 1, sizeof(buffer), ptr);
        total += count;

        // Write to the connection and compute the checksum
        for (i = 0; i < count; i++)
        {
            raspbootSerialPut(serial, buffer[i]);
            checksum += buffer[i];
        }

        // Print info to the Raspboot client
        printf("\t%d/%d bytes written\n", total, size);

        if (count < OUTPUT_BLOCK_SIZE)
        {
            break;
        }
    }

    // Send the checksum value
    printf("\tChecksum = %d, confirming with server...\n", checksum);
    raspbootSerialPut(serial, checksum);
    raspbootSerialFlush(serial);

    // Verify the checksum with the device
    U8 checkResult;
    raspbootSerialGet(serial, &checkResult);
    if (checkResult == COMMAND_TRANSFER_CHECKP)
    {
        printf("\tChecksum passed\n");
    }
    else
    {
        printf("\tChecksum failed. Retry transfer (y/n)?\n> ");
        return -1;
    }

    // Write the end command byte
    U8 endCommand = (args->go == 0 ?
                        COMMAND_TRANSFER_END : COMMAND_TRANSFER_END_R);
    
    raspbootSerialPut(serial, endCommand);

    return 0;
}