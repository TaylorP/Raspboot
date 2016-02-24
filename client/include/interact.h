#ifndef INTERACT_H
#define INTERACT_H

#include <common/types.h>

#include "args.h"
#include "serial.h"

#define TOKENIZER_SIZE 16

/** 
 * Performs an action in interactive mode
 *
 * @param   serial  The serial port to write commands to
 * @param   args    The Raspboot configuration
 * @param   input   The last input command string
 *
 * @return  0 on success, 1 on exit, else an error code
 */
S32 raspbootInteract(Raspboot_Serial* serial,
                     Raspboot_Args* args,
                     char* input);

#endif