#ifndef UART_H
#define UART_H

#include <common/types.h>

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

/**
 * Initializes the UART0 serial IO port
 */
void raspbootUartInit(void);

/**
 * Writes a byte to the UART; blocks until the UART is able to write
 *
 * @param   byte    The value to write
 *
*/
void raspbootUartPut(const U8 byte);

/**
 * Reads a byte from the UART; blocks until there's something to read
 *
 * @return  The byte read from the UART
 */
U8 raspbootUartGet(void);

#endif