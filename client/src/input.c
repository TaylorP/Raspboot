#include <stdio.h>
#include <common/command.h>
#include "input.h"

S32 raspbootInputGo(Raspboot_Serial* serial)
{
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

    return 0;
}

S32 raspbootInputGet(Raspboot_Serial* serial)
{
    U8 count = 0;
    while (1)
    {
        U8 byte;
        raspbootSerialGet(serial, &byte);
        if (byte == COMMAND_INTERACT_END)
        {
            break;
        }

        if (count != 0)
        {
            if (count % 16 == 0)
            {
                printf("\n\t");
            }
            else if (count % 2 == 0)
            {
                printf(" ");
            }
        }
        else
        {
            printf("\t");
        }

        printf("%02x", byte);
        count++;
    }

    printf("\n");
    return 0;
}