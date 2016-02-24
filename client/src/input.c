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