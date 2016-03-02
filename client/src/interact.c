#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common/command.h>
#include "input.h"
#include "interact.h"
#include "output.h"

S32 _raspbootInteractTokens(char* command, char** buffer, const U32 length);
void _raspbootInteractUsage();

S32 raspbootInteract(Raspboot_Serial* serial,
                     Raspboot_Args* args,
                     char* input)
{
    // Tokenize input commands
    char* tokens[TOKENIZER_SIZE];
    U32 count = _raspbootInteractTokens(input, tokens, TOKENIZER_SIZE);

    // Return early for empty input
    if (count == 0)
    {
        return -1;
    }

    // Process the `go` command
    if (strcmp(tokens[0], "go") == 0)
    {
        // Extract location parameter, if any is provided
        U32 location = args->location;
        if (count == 3 && (strcmp(tokens[1], "-l") == 0))
        {
            location = (U32)strtol(tokens[2], NULL, 0);
        }
        else if (count != 1)
        {
            fprintf(stderr,
                "\tInvalid command format, expected: go [-l loc]\n");
            return -1;
        }

        // Run the code and wait for the device to return
        raspbootOutputGo(serial, args, location);
        raspbootInputGo(serial);
    }
    else if (strcmp(tokens[0], "quit") == 0)
    {
        return 1;
    }
    else if (strcmp(tokens[0], "help") == 0)
    {
        _raspbootInteractUsage();
        return 0;
    }
    else if (strcmp(tokens[0], "get") == 0)
    {
        U32 location;
        U32 byteCount = 4;

        if (count == 5)
        {
            if (strcmp(tokens[1], "-l") == 0)
            {
                location = (U32)strtol(tokens[2], NULL, 0);
            }
            else if (strcmp(tokens[3], "-l") == 0)
            {
                location = (U32)strtol(tokens[4], NULL, 0);
            }
            else
            {
                fprintf(stderr,
                    "\tInvalid command format, missing location parameter\n");
                return -1;
            }

            if (strcmp(tokens[1], "-c") == 0)
            {
                byteCount = (U32)strtol(tokens[2], NULL, 0);
            }
            else if (strcmp(tokens[3], "-c") == 0)
            {
                byteCount = (U32)strtol(tokens[4], NULL, 0);
            }
            else
            {
                fprintf(stderr,
                    "\tInvalid command format. Expect -l and -c flags\n");
                return -1;
            }
        }
        else if (count == 3 && (strcmp(tokens[1], "-l") == 0))
        {
            location = (U32)strtol(tokens[2], NULL, 0);
        }
        else
        {
            fprintf(stderr,
                "\tInvalid command format, expected: get -l loc [-c count]\n");
            return -1;   
        }

        if (byteCount < 1 || byteCount > 64)
        {
            fprintf(stderr,
                "\tInvalid command value. Count must be between 1 and 64\n");
            return -1;
        }

        raspbootOutputGet(serial, args, location, byteCount);
        raspbootInputGet(serial, byteCount);
    }
    else if (strcmp(tokens[0], "set") == 0)
    {
        U32 location;
        U8 value = 0;

        if (count == 5)
        {
            if (strcmp(tokens[1], "-l") == 0)
            {
                location = (U32)strtol(tokens[2], NULL, 0);
            }
            else if (strcmp(tokens[3], "-l") == 0)
            {
                location = (U32)strtol(tokens[4], NULL, 0);
            }
            else
            {
                fprintf(stderr,
                    "\tInvalid command format, missing location parameter\n");
                return -1;
            }

            if (strcmp(tokens[1], "-v") == 0)
            {
                value = (U8)strtol(tokens[2], NULL, 0);
            }
            else if (strcmp(tokens[3], "-v") == 0)
            {
                value = (U8)strtol(tokens[4], NULL, 0);
            }
            else
            {
                fprintf(stderr,
                    "\tInvalid command format. Expect -l and -v flags\n");
                return -1;
            }
        }
        else if (count == 3 && (strcmp(tokens[1], "-l") == 0))
        {
            location = (U32)strtol(tokens[2], NULL, 0);
            value = 0;
        }
        else
        {
            fprintf(stderr,
                "\tInvalid command format, expected: set -l loc [-v value]\n");
            return -1;   
        }

        raspbootOutputSet(serial, args, location, value);
    }
    else if (strcmp(tokens[0], "setw") == 0)
    {
        U32 location;
        U32 value = 0;

        if (count == 5)
        {
            if (strcmp(tokens[1], "-l") == 0)
            {
                location = (U32)strtol(tokens[2], NULL, 0);
            }
            else if (strcmp(tokens[3], "-l") == 0)
            {
                location = (U32)strtol(tokens[4], NULL, 0);
            }
            else
            {
                fprintf(stderr,
                    "\tInvalid command format, missing location parameter\n");
                return -1;
            }

            if (strcmp(tokens[1], "-v") == 0)
            {
                value = (U32)strtol(tokens[2], NULL, 0);
            }
            else if (strcmp(tokens[3], "-v") == 0)
            {
                value = (U32)strtol(tokens[4], NULL, 0);
            }
            else
            {
                fprintf(stderr,
                    "\tInvalid command format. Expect -l and -v flags\n");
                return -1;
            }
        }
        else if (count == 3 && (strcmp(tokens[1], "-l") == 0))
        {
            location = (U32)strtol(tokens[2], NULL, 0);
            value = 0;
        }
        else
        {
            fprintf(stderr,
                "\tInvalid command format, expected: set -l loc [-v value]\n");
            return -1;   
        }

        if (location % 4 != 0)
        {
            fprintf(stderr,
                "\tInvalid command value. Location must be word aligned\n");
            return -1;
        }


        raspbootOutputSetW(serial, args, location, value);
    }
    else
    {
        fprintf(stderr,
            "\tUnknown command `%s`\n", tokens[0]);
        return -1;   
    }

    return 0;
}

S32 _raspbootInteractTokens(char* command, char** buffer, const U32 length)
{
    if (command == 0)
    {
        return -1;
    }

    static const char* seperators = " \t\n";
    char* token = strtok(command, seperators);
    U32 index = 0;

    while (token)
    {
        buffer[index++] = token;
        token = strtok((char*)0, seperators);

        if (index >= length)
        {
            return -1;
        }
    }

    return index;
}

void _raspbootInteractUsage()
{
    printf("\tThe following commands are supported. Arguments listed in \n"\
           "\t[square brackets] are optional.\n\n"\
           "\tget -l loc [-c count]\tReads `count` bytes at address `loc`. The\n"\
           "\t\t\t\tdefault value of `count` is 4.\n\n" \
           "\tgo [-l loc]\t\tExecutes code at address `loc`. The default\n"\
           "\t\t\t\tvalue of `loc` is the address passed in over\n"\
           "\t\t\t\tthe command line.\n\n"\
           "\thelp\t\t\tPrints this help message.\n\n"\
           "\tquit\t\t\tExits Raspboot.\n");
}