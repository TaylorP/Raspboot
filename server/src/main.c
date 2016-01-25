#include "memory.h"

#define GPFSEL1         0x20200004
#define GPSET0          0x2020001C
#define GPCLR0          0x20200028
#define GPPUD           0x20200094
#define GPPUDCLK0       0x20200098

#define UART0_BASE      0x20201000
#define UART0_DR        (UART0_BASE+0x00)
#define UART0_RSRECR    (UART0_BASE+0x04)
#define UART0_FR        (UART0_BASE+0x18)
#define UART0_ILPR      (UART0_BASE+0x20)
#define UART0_IBRD      (UART0_BASE+0x24)
#define UART0_FBRD      (UART0_BASE+0x28)
#define UART0_LCRH      (UART0_BASE+0x2C)
#define UART0_CR        (UART0_BASE+0x30)
#define UART0_IFLS      (UART0_BASE+0x34)
#define UART0_IMSC      (UART0_BASE+0x38)
#define UART0_RIS       (UART0_BASE+0x3C)
#define UART0_MIS       (UART0_BASE+0x40)
#define UART0_ICR       (UART0_BASE+0x44)
#define UART0_DMACR     (UART0_BASE+0x48)
#define UART0_ITCR      (UART0_BASE+0x80)
#define UART0_ITIP      (UART0_BASE+0x84)
#define UART0_ITOP      (UART0_BASE+0x88)
#define UART0_TDR       (UART0_BASE+0x8C)

void uartInit()
{
    __str(UART0_CR,0);

    U32 pinVal = __ldr(GPFSEL1);
    pinVal &= ~(7<<12);
    pinVal |= 4<<12;
    pinVal &= ~(7<<15);
    pinVal |= 4<<15;

    __str(GPFSEL1, pinVal);
    __str(GPPUD, 0);
    __str(GPPUDCLK0, 0);

    __str(UART0_ICR, 0x7FF);
    __str(UART0_IBRD, 1);
    __str(UART0_FBRD, 40);
    __str(UART0_LCRH, 0x70);
    __str(UART0_CR, 0x301);
}

S32 uartGet()
{
    while ((__ldr(UART0_FR) & 0x10) != 0)
    {
    }

    return __ldr(UART0_DR);
}

void main()
{
    uartInit();

    while(1)
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

        ((void (*)(void)) address)();
    }
}
