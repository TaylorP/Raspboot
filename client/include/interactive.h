#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <common/types.h>

#include "args.h"
#include "serial.h"

#define TOKENIZER_SIZE 16

S32 raspbootInteractive(Raspboot_Serial* serial,
                        Raspboot_Args* args,
                        char* input);

#endif