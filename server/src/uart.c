#include "memory.h"
#include "uart.h"

void raspbootUartInit()
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

void raspbootUartPut(const U8 byte)
{
    while ((__ldr(UART0_FR) & 0x20) != 0)
    {
    }

    __strb(UART0_DR, byte);
}

U8 raspbootUartGet()
{
    while ((__ldr(UART0_FR) & 0x10) != 0)
    {
    }

    return __ldrb(UART0_DR);
}