#include <stdio.h>
#include <stdlib.h>

#include <common/mode.h>
#include <common/command.h>

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
        raspbootOutputMode(&serial, MODE_TRANSFER);
        raspbootOutputBinary(&serial, &args);
    }

    raspbootOutputMode(&serial, MODE_INTERACT);
    raspbootSerialFlush(&serial);

    if (args.terminate == 0)
    {
        U8 inputBuffer[128];
        U8 inputIndex = 0;
        printf("> ");

        while (1)
        {
            inputBuffer[inputIndex++] = getchar();

            if (inputBuffer[inputIndex - 1] == '\n')
            {
                inputBuffer[inputIndex] = '\0';
                inputIndex = 0;

                if (inputBuffer[0] == 'g' && inputBuffer[1] == 'o')
                {
                    printf("\tExecuting code at memory address 0x%x\n",
                           args.location);
                    raspbootSerialPut(&serial, COMMAND_INTERACT_GO);
                    raspbootSerialFlush(&serial);
                }
                printf("> ");
            }
        }
    }
        
    raspbootArgsDestroy(&args);
    exit(0);
}
