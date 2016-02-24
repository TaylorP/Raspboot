#include <common/command.h>
#include <common/mode.h>

#include "interact.h"
#include "uart.h"

S32 raspbootInteractMode(U32* mode)
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
            return PROCESS_INTERACT_EXECUTE; 
        }

        return PROCESS_INTERACT_ERROR;
    }
}