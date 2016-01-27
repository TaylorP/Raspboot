#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "serial.h"

S32 _raspbootInitTerminal(Raspboot_Serial* serial, Raspboot_Args* args)
{
    // Initialize the tty structure
    struct termios* tty = &(serial->tty);
    memset(tty, 0, sizeof(*tty));

    // Query initial TTY state
    if (tcgetattr(serial->connection, tty) != 0)
    {
        fprintf(stderr,
                "raspboot: error querying tty attributes: %s",
                strerror(errno));
        return -1;
    }

    // Set the TTY speed
    cfsetospeed(tty, B115200);
    cfsetispeed(tty, B115200);

    // Set other terminal flags (char size, breaks, etc)
    tty->c_cflag = (tty->c_cflag & ~CSIZE) | CS8;
    tty->c_iflag &= ~IGNBRK;
    tty->c_lflag = 0;
    tty->c_oflag = 0;

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
        fprintf (stderr,
                 "raspboot: error when setting tty attributes: %s",
                 strerror(errno));
        return -1;
    }

    return 0;
}

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
        fprintf(stderr,
                "raspboot: error opening %s: %s\n",
                args->port, strerror(errno));
        return -1;
    }

    // Set TTY settings for the device
    return _raspbootInitTerminal(serial, args);
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