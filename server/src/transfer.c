#include <common/command.h>
#include <common/mode.h>

#include "transfer.h"
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
            return PROCESS_TRANSFER_SUCCESS;
        }
        else if (command == COMMAND_TRANSFER_END_R)
        {
            *address = addr;
            return PROCESS_TRANSFER_EXECUTE;
        }

        return PROCESS_TRANSFER_ERROR;
    }

    return PROCESS_TRANSFER_ERROR;
}