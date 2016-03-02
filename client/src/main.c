#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <common/mode.h>

#include "args.h"
#include "console.h"
#include "interact.h"
#include "output.h"
#include "serial.h"

int main(int argc, char** argv)
{
    Raspboot_Args args;
    Raspboot_Serial serial;

    if (raspbootArgsInit(&args) != 0 ||
        raspbootArgsParse(&args, argc, argv) < 0)
    {
        raspbootConsoleFatal(0, "Unable to parse arguments.\n");;
    }

    if (args.help)
    {
        raspbootArgsUsage();
        exit(0);
    }

    if (raspbootSerialInit(&serial, &args) != 0)
    {
        raspbootConsoleFatal(0, "Unable to initialize serial connection\n");
    }

    if (args.binary != 0)
    {
        raspbootOutputMode(&serial, &args, MODE_TRANSFER);
        while (raspbootOutputBinary(&serial, &args) != 0)
        {
            char retry = getchar();
            while (isspace(retry))
            {
                retry = getchar();
            }

            if (retry != 'Y' && retry != 'y')
            {
                raspbootConsoleWarn(&args, "Transfer Aborted.\n");
            }
        }
    }

    raspbootOutputMode(&serial, &args, MODE_INTERACT);
    raspbootSerialFlush(&serial);

    if (args.terminate == 0)
    {
        char inputBuffer[128];
        U8 inputIndex = 0;
        printf("> ");

        while (1)
        {
            inputBuffer[inputIndex++] = getchar();

            if (inputBuffer[inputIndex - 1] == '\n' || inputIndex == 126)
            {
                inputBuffer[inputIndex] = '\0';
                inputIndex = 0;
                if (raspbootInteract(&serial, &args, inputBuffer) == 1)
                {
                    raspbootConsoleInfo(&args, "Exiting Raspboot\n");
                    break;
                }
                printf("> ");
            }
        }
    }
        
    raspbootArgsDestroy(&args);
    exit(0);
}
