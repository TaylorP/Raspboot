#include <common/command.h>
#include <common/mode.h>
#include <common/types.h>

#include "memory.h"
#include "transfer.h"
#include "uart.h"

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

S32 processInteract()
{
    return 0;
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
                if (raspbootTransferMode(&address) == 1)
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
