#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <common/command.h>
#include "interact.h"
#include "output.h"

S32 _raspbootTokenizer(char* command, char** buffer, const U32 length)
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

S32 raspbootInteract(Raspboot_Serial* serial,
                     Raspboot_Args* args,
                     char* input)
{
    char* tokens[TOKENIZER_SIZE];
    U32 count = _raspbootTokenizer(input, tokens, TOKENIZER_SIZE);

    if (count == 0)
    {
        return -1;
    }

    if (strcmp(tokens[0], "go") == 0)
    {
        U32 location = args->location;

        if (count == 3 && (strcmp(tokens[1], "-l") == 0))
        {
            location = (U32)strtol(tokens[2], NULL, 0);
        }
        else if (count != 1)
        {
            fprintf(stderr,
                "raspboot: invalid command format\n");
            return -1;
        }

        printf("Executing code at memory address 0x%x\n\n", location);
        raspbootOutputGo(serial, location);
        
        while (1)
        {
            U8 byte;
            raspbootSerialGet(serial, &byte);
            if (byte == COMMAND_INTERACT_END)
            {
                break;
            }
            printf("%c", byte);
        }
    }

    return 0;
}