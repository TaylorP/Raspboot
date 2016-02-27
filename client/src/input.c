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

S32 raspbootInputGet(Raspboot_Serial* serial, const U8 count)
{
    U8 read = 0;
    while (read < count)
    {
        U8 byte;
        raspbootSerialGet(serial, &byte);
        
        if (read != 0)
        {
            if (read % 16 == 0)
            {
                printf("\n\t");
            }
            else if (read % 2 == 0)
            {
                printf(" ");
            }
        }
        else
        {
            printf("\t");
        }

        printf("%02x", byte);
        read++;
    }

    printf("\n");
    return 0;
}