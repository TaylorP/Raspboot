#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "console.h"
#include "serial.h"

S32 _baudRate(const U32 baud);
S32 _raspbootInitTerminal(Raspboot_Serial* serial, Raspboot_Args* args);

S32 raspbootSerialInit(Raspboot_Serial* serial, Raspboot_Args* args)
{
    // Return early if the arguments are null
    if (serial == 0 || args == 0)
    {
        return -1;
    }

    // Initialiaze the serial port output buffer
    serial->bufferIndex = 0;

    // Open a connection the device specified in the port argument
    serial->connection = open(args->port, O_RDWR | O_NOCTTY | O_SYNC);
    if (serial->connection < 0)
    {
        raspbootConsoleError(args, "Unable to open port `%s`: %s\n",
                args->port, strerror(errno));
        return -1;
    }

    // Set TTY settings for the device
    return _raspbootInitTerminal(serial, args);
}

S32 raspbootSerialGet(Raspboot_Serial* serial, U8* byte)
{
    if (read(serial->connection, byte, 1) == 1)
    {
        return 0;
    }

    return -1;
}

S32 raspbootSerialPut(Raspboot_Serial* serial, const U8 byte)
{
    serial->buffer[serial->bufferIndex++] = byte;

    if (serial->bufferIndex == SERIAL_BUFFER_SIZE)
    {
        return raspbootSerialFlush(serial);
    }

    return 0;
}

S32 raspbootSerialPutW(Raspboot_Serial* serial, const U32 word)
{
    U32 i;
    for (i = 0; i < 4; i++)
    {
        U8 b = (U8)((word >> ((3-i) << 3)) & 0xff);
        S32 result = raspbootSerialPut(serial, b);
        if (result != 0)
        {
            return result;
        }
    }

    return 0;
}

S32 raspbootSerialPutN(Raspboot_Serial* serial, U8* bytes, const U32 n)
{
    U32 iter = 0;
    while (iter < n)
    {
        serial->buffer[serial->bufferIndex++] = bytes[iter];

        if (serial->bufferIndex == SERIAL_BUFFER_SIZE)
        {
            S32 result = raspbootSerialFlush(serial);
            if (result != 0)
            {
                return result;
            }
        }

        iter++;
    }

    return 0;
}

S32 raspbootSerialFlush(Raspboot_Serial* serial)
{
    S32 result = write(serial->connection, serial->buffer, serial->bufferIndex);
    usleep(serial->bufferIndex * 100);
    serial->bufferIndex = 0;
    
    return result;
}

S32 raspbootSerialClear(Raspboot_Serial* serial, const U32 queue)
{
    return tcflush(serial->connection, queue);
}

S32 _raspbootBaudRate(const U32 baud)
{
    switch (baud)
    {
        case 0:
            return B0;
        case 50:
            return B50;
        case 75:
            return B75;
        case 110:
            return B110;
        case 134:
            return B134;
        case 150:
            return B150;
        case 200:
            return B200;
        case 300:
            return B300;
        case 600:
            return B600;
        case 1200:
            return B1200;
        case 1800:
            return B1800;
        case 2400:
            return B2400;
        case 4800:
            return B4800;
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        case 230400:
            return B230400;

        default:
            return -1;
    }
}

S32 _raspbootInitTerminal(Raspboot_Serial* serial, Raspboot_Args* args)
{
    // Initialize the tty structure
    struct termios* tty = &(serial->tty);
    memset(tty, 0, sizeof(*tty));

    // Query initial TTY state
    if (tcgetattr(serial->connection, tty) != 0)
    {
        raspbootConsoleError(args, "Unable to query tty attributes. (%s)\n",
                strerror(errno));
        return -1;
    }

    S32 baud = _raspbootBaudRate(args->speed);
    if (baud < 0)
    {
        raspbootConsoleWarn(args,
                "Invalid baud rate `%d`. Using default of 115,200.\n",
                args->speed);
        baud = B115200;
    }

    // Set the TTY speed
    cfsetospeed(tty, baud);
    cfsetispeed(tty, baud);

    // Set other terminal flags (char size, breaks, etc)
    tty->c_cflag = (tty->c_cflag & ~CSIZE) | CS8;
    tty->c_iflag &= ~IGNBRK;
    tty->c_lflag = 0;
    tty->c_oflag = 0;

    // Set blocking parameters
    tty->c_cc[VMIN]  = 1;
    tty->c_cc[VTIME] = 5;

    // Disable xon/xoff control
    tty->c_iflag &= ~(IXON | IXOFF | IXANY);

    // Disable parity and CTS/RTs
    tty->c_cflag |= (CLOCAL | CREAD);
    tty->c_cflag &= ~(PARENB | PARODD);
    tty->c_cflag &= ~CSTOPB;
    tty->c_cflag &= ~CRTSCTS;

    // Apply the settings to the TTY
    if (tcsetattr(serial->connection, TCSANOW, tty) != 0)
    {
        raspbootConsoleError(args, "Unable to set tty attributes. (%s)\n",
                strerror(errno));
        return -1;
    }

    // Clear all pending data from the TTY
    raspbootSerialClear(serial, TCIOFLUSH);

    return 0;
}

