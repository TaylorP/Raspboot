#include <common/command.h>
#include <common/mode.h>
#include <common/types.h>

#include "memory.h"
#include "uart.h"

U32 readBinary()
{
    U32 address = 0;

    U32 i;
    for (i = 0; i < 4; i++)
    {
        address = address << 8;
        address |= uartGet();
    }

    U32 count = 0;
    for (i = 0; i < 4; i++)
    {
        count = count << 8;
        count |= uartGet();
    }

    U8* mem = (U8*)(address);
    for (i = 0; i < count; i++)
    {
        *mem = (U8)(uartGet());
        mem += 1;
    }

    return address;
}

S32 processAbort(U32* mode)
{
    U32 input = uartGet();
    if (input == MODE_TRANSFER || input == MODE_INTERACT)
    {
        *mode = input;
        return 0;
    }

    return -1;
}

S32 processTransfer(U32* address)
{
    U32 input = uartGet();

    if (input == COMMAND_TRANSFER_INIT)
    {
        U32 addr = readBinary();
        U8 command = uartGet();
        if (command == COMMAND_TRANSFER_END)
        {
            *address = addr;
            return 0;
        }
        else if (command == COMMAND_TRANSFER_END_R)
        {
            *address = addr;
            return 1;
        }

        return -1;
    }

    return -1;
}

S32 processInteract()
{
    return 0;
}

void main()
{
    uartInit();

    U32 mode = MODE_ABORT;
    U32 address = 0;

    while(1)
    {        
        switch (mode)
        {
            case MODE_ABORT:
            {
                processAbort(&mode);
                break;
            }

            case MODE_TRANSFER:
            {
                if (processTransfer(&address) == 1)
                {
                    ((void (*)(void)) address)();
                }
                
                break;
            }

            case MODE_INTERACT:
            {
                processInteract();
                break;
            }

            default:
                break;
        }
    }
}
