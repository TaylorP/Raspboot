#ifndef INPUT_H
#define INPUT_H

#include <common/types.h>
#include "serial.h"

/**
 * Reads input and prints it to stdout. Blocks until an END command is sent
 *
 * @param   serial  The Raspboot_Serial connection to read from
 *
 * @return  0 on success, else an error code
 */
S32 raspbootInputGo(Raspboot_Serial* serial);

#endif