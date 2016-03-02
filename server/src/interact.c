#include <common/command.h>
#include <common/mode.h>

#include "interact.h"
#include "uart.h"

S32 raspbootInteractMode(U32* address, U32* mode)
{
    U8 input = raspbootUartGet();

    if (input == MODE_INTERACT)
    {
        return PROCESS_INTERACT_SUCCESS;
    }
    else if (input == MODE_TRANSFER)
    {
        *mode = input;
        return PROCESS_INTERACT_SUCCESS;
    }
    else if (input == MODE_ABORT)
    {
        *mode = input;
        return PROCESS_INTERACT_ERROR;
    }

    if (input == COMMAND_INTERACT_GO)
    {
        U32 addr = 0;

        U32 i;
        for (i = 0; i < 4; i++)
        {
            addr = addr << 8;
            addr |= raspbootUartGet();
        }

        *address = addr;
        
        return PROCESS_INTERACT_EXECUTE; 
    }
    else if (input == COMMAND_INTERACT_GET)
    {
        U8 count = raspbootUartGet();
        U32 addr = 0;

        U32 i;
        for (i = 0; i < 4; i++)
        {
            addr = addr << 8;
            addr |= raspbootUartGet();
        }

        U8* mem = (U8*)(addr);
        for (i = 0; i < count; i++, mem++)
        {
            raspbootUartPut(*mem);
        }

        return PROCESS_INTERACT_SUCCESS;
    }
    else if (input == COMMAND_INTERACT_SET)
    {
        U32 addr = 0;
        U32 value = raspbootUartGet();

        U32 i;
        for (i = 0; i < 4; i++)
        {
            addr = addr << 8;
            addr |= raspbootUartGet();
        }

        U8* mem = (U8*)(addr);
        *mem = value;

        return PROCESS_INTERACT_SUCCESS;
    }
    else if (input == COMMAND_INTERACT_SET_W)
    {
        U32 addr = 0;
        U32 value = 0;
        U32 i;

        for (i = 0; i < 4; i++)
        {
            value = value << 8;
            value |= raspbootUartGet();
        }

        for (i = 0; i < 4; i++)
        {
            addr = addr << 8;
            addr |= raspbootUartGet();
        }

        U32* mem = (U32*)(addr);
        *mem = value;

        return PROCESS_INTERACT_SUCCESS;
    }

    return PROCESS_INTERACT_ERROR;
}