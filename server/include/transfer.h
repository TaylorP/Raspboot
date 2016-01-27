#ifndef TRANSFER_H
#define TRANSFER_H

#include <common/types.h>

#define PROCESS_TRANSFER_ERROR      -1
#define PROCESS_TRANSFER_SUCCESS    0
#define PROCESS_TRANSFER_EXECUTE    1

/**
 * Runs the state machine for transfer mode
 *
 * @param   address     The memory address containing the transfered bytes
 * @param   mode        A pointer to the mode value
 *
 * @returns A transfer mode return code
 */
S32 raspbootTransferMode(U32* address, U32* mode);

#endif