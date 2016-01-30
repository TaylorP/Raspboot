#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>
#include <common/types.h>
#include "args.h"

#define SERIAL_BUFFER_SIZE  1024

/**
 * A structure for storing information about the serial
 * connection used by Raspboot.
 */
typedef struct
{
    /// The terminal io structure for the file descritor
    struct termios tty;

    /// The file descriptor for the serial port connection
    S32 connection;   

    /// The serial buffer index
    U16 bufferIndex;

    /// The serial output buffer
    U8 buffer[SERIAL_BUFFER_SIZE];
} Raspboot_Serial;

/**
 * Initializes a serial connection instance
 *
 * @param   serial  The Raspboot_Serial connection to initialize
 * @param   args    The Raspboot_Args instance describing the connection
 *
 * @return  0 on success, else an error code
 */
S32 raspbootSerialInit(Raspboot_Serial* serial, Raspboot_Args* args);

/**
 * Reads a single byte from a serial connection
 *
 * @param   serial  The Raspboot_Serial connection to read from
 * @param   byte    The byte to read into
 *
 * @return  0 on sucess, else an error code
 */
S32 raspbootSerialGet(Raspboot_Serial* serial, U8* byte);

/**
 * Writes a single byte to a serial connection
 *
 * @param   serial  The Raspboot_Serial connection to write to
 * @param   byte    The byte to write
 *
 * @return  0 on success, else an error code
 */
S32 raspbootSerialPut(Raspboot_Serial* serial, const U8 byte);

/**
 * Writes a 32 bit value to a serial connection
 *
 * @param   serial  The Raspboot_Serial connection to write to
 * @param   word    The 32 bit value to write
 *
 * @return  0 on success, else an error code
 */
S32 raspbootSerialPutW(Raspboot_Serial* serial, const U32 word);

/**
 * Writes n bytes to a serial connection
 *
 * @param   serial  The Raspboot_Serial connection to write to
 * @param   bytes   The byte array to write
 * @param   n       The number of bytes to write
 *
 * @return  0 on success, else an error code
 */
S32 raspbootSerialPutN(Raspboot_Serial* serial, U8* bytes, const U32 n);

/**
 * Flushes pending writes to the serial connection
 *
 * @param   serial  The Raspboot_Serial connection to flush
 *
 * @return 0 on success, else an error code
 */
S32 raspbootSerialFlush(Raspboot_Serial* serial);

/**
 * Clears unprocessed data the serial connection
 *
 * @param   serial  The Raspboot_Serial connection to clear
 * @param   queue   The termios queue selector to clear
 *
 * @return  0 on success, else an error code
 */
S32 raspbootSerialClear(Raspboot_Serial* serial, const U32 queue);

#endif