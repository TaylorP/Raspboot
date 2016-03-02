#ifndef OUTPUT_H
#define OUTPUT_H

#include "serial.h"

#define OUTPUT_BLOCK_SIZE 1024

/**
 * Writes a mode change to the serial connection
 *
 * @param   serial  The Raspboot_Serial connection to write to
 * @param   args    The Raspboot_Args instance for configuration
 * @param   mode    The mode to send
 *
 * @return  0 on success, else an error code
 */
S32 raspbootOutputMode(Raspboot_Serial* serial,
                       Raspboot_Args* args,
                       const U8 mode);

/**
 * Writes a binary file to the serial connection
 *
 * @param   serial  The Raspboot_Serial connection to write to
 * @param   args    The Raspboot_Args instance describing the binary file
 *
 * @return  0 on success, else an error code
 */
S32 raspbootOutputBinary(Raspboot_Serial* serial, Raspboot_Args* args);

/**
 * Writes a go command to the serial connection
 *
 * @param   serial  The Raspboot_Serial connection to write to
 * @param   args    The Raspboot_Args instance for configuration
 * @param   address The address to pass along with the go command
 *
 * @return  0 on sucess, else an error code
 */
S32 raspbootOutputGo(Raspboot_Serial* serial,
                     Raspboot_Args* args,
                     const U32 address);    

/**
 * Writes a get command to the serial connection
 *
 * @param   serial  The Raspboot_Serial connection to write to
 * @param   args    The Raspboot_Args instance for configuration
 * @param   address The address to read from
 * @param   count   The number of bytes to read
 *
 * @return  0 on sucess, else an error code
 */
S32 raspbootOutputGet(Raspboot_Serial* serial,
                      Raspboot_Args* args,
                      const U32 address,
                      const U8 count);

/**
 * Writes a set byte command to the serial connection
 *
 * @param   serial  The Raspboot_Serial connection to write to
 * @param   args    The Raspboot_Args instance for configuration
 * @param   address The address to write to
 * @param   value   The value to write to the address
 *
 * @return  0 on sucess, else an error code
 */
S32 raspbootOutputSet(Raspboot_Serial* serial,
                      Raspboot_Args* args,
                      const U32 address,
                      const U8 value);

/**
 * Writes a set word command to the serial connection
 *
 * @param   serial  The Raspboot_Serial connection to write to
 * @param   args    The Raspboot_Args instance for configuration
 * @param   address The address to write to
 * @param   value   The value to write to the address
 *
 * @return  0 on sucess, else an error code
 */
S32 raspbootOutputSetW(Raspboot_Serial* serial,
                       Raspboot_Args* args,
                       const U32 address,
                       const U32 value);

#endif