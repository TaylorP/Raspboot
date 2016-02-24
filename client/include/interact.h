#ifndef INTERACT_H
#define INTERACT_H

#include <common/types.h>

#include "args.h"
#include "serial.h"

#define TOKENIZER_SIZE 16

S32 raspbootInteract(Raspboot_Serial* serial,
                     Raspboot_Args* args,
                     char* input);

#endif