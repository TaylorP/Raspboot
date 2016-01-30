#include <common/command.h>
#include <common/mode.h>

#include "transfer.h"
#include "uart.h"

S32 _raspbootTransferBinary(U32* address)
{
    U32 addr = 0;

    U32 i;
    for (i = 0; i < 4; i++)
    {
        addr = addr << 8;
        addr |= raspbootUartGet();
    }

    U32 count = 0;
    for (i = 0; i < 4; i++)
    {
        count = count << 8;
        count |= raspbootUartGet();
    }

    U8* mem = (U8*)(addr);
    U8 checksum = 0;
    for (i = 0; i < count; i++)
    {
        U8 byte = (U8)(raspbootUartGet());
        *mem = byte;
        mem += 1;
        checksum += byte;
    }

    U8 clientsum = (U8)(raspbootUartGet());
    if (clientsum != checksum)
    {
        raspbootUartPut(COMMAND_TRANSFER_CHECKF);
        return -1;
    }

    raspbootUartPut(COMMAND_TRANSFER_CHECKP);

    *address = addr;
    return 0;
}

S32 raspbootTransferMode(U32* address, U32* mode)
{
    U8 input = raspbootUartGet();

    while (1)
    {
        if (input == MODE_TRANSFER)
        {
            input = raspbootUartGet();
            continue;
        }
        else if (input == MODE_INTERACT)
        {
            *mode = input;
            return PROCESS_TRANSFER_SUCCESS;
        }
        else if (input == MODE_ABORT)
        {
            *mode = input;
            return PROCESS_TRANSFER_ERROR;
        }

        if (input == COMMAND_TRANSFER_INIT)
        {
            U32 addr = 0;
            if (_raspbootTransferBinary(&addr) == 0)
            {
                U8 command = raspbootUartGet();

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
            }

            return PROCESS_TRANSFER_ERROR;
        }

        return PROCESS_TRANSFER_ERROR;
    }
}