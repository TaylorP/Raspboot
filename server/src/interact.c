#include <common/command.h>
#include <common/mode.h>

#include "interact.h"
#include "uart.h"

S32 raspbootInteractMode(U32* address, U32* mode)
{
    U8 input = raspbootUartGet();

    while (1)
    {
        if (input == MODE_INTERACT)
        {
            input = raspbootUartGet();
            continue;
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

        return PROCESS_INTERACT_ERROR;
    }
}