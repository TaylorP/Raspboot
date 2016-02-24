#ifndef INTERACT_H
#define INTERACT_H

#include <common/types.h>

#define PROCESS_INTERACT_ERROR      -1
#define PROCESS_INTERACT_SUCCESS    0
#define PROCESS_INTERACT_EXECUTE    1

/**
 * Runs the state machine for interact mode
 *
 * @param   address     The memory address containing the transfered bytes
 * @param   mode        A pointer to the mode value
 *
 * @returns An interaction mode return code
 */
S32 raspbootInteractMode(U32* address, U32* mode);

#endif