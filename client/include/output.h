#ifndef OUTPUT_H
#define OUTPUT_H

#include "serial.h"

/**
 * Writes a binary file to the serial connection.
 *
 * @param   serial  The Raspboot_Serial connection to write to
 * @param   args    The Raspboot_Args instance describing the binary file
 *
 * @return  0 on success, else an error code
 */
S32 raspbootOutputBinary(Raspboot_Serial* serial, Raspboot_Args* args);

#endif