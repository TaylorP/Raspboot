#ifndef INPUT_H
#define INPUT_H

#include <common/types.h>
#include "serial.h"

/**
 * Reads go input and prints it to stdout. Waits for an END command.
 *
 * @param   serial  The Raspboot_Serial connection to read from
 *
 * @return  0 on success, else an error code
 */
S32 raspbootInputGo(Raspboot_Serial* serial);

/**
 * Reads get response data and prints it to stdout. Waits for an END command.
 *
 * @param   serial  The Raspboot_Serial connection to read from
 *
 * @return  0 on success, else an error code
 */
S32 raspbootInputGet(Raspboot_Serial* serial);

#endif