#include <common/command.h>
#include <common/mode.h>
#include <common/types.h>

#include "memory.h"
#include "interact.h"
#include "transfer.h"
#include "uart.h"

extern void _enterAddress(U32);

S32 processAbort(U32* mode)
{
    U32 input = raspbootUartGet();
    if (input == MODE_TRANSFER || input == MODE_INTERACT)
    {
        *mode = input;
        return 0;
    }

    return -1;
}

void main()
{
    raspbootUartInit();

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
                if (raspbootTransferMode(&address, &mode)
                        == PROCESS_TRANSFER_EXECUTE)
                {
                    _enterAddress(address);
                }
                
                break;
            }

            case MODE_INTERACT:
            {
                S32 result = raspbootInteractMode(&mode);

                if (result == PROCESS_INTERACT_EXECUTE)
                {
                    _enterAddress(address);
                    raspbootUartPut(COMMAND_INTERACT_END);
                }

                break;
            }

            default:
                break;
        }
    }
}
