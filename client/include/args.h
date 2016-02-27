#ifndef ARGS_H
#define ARGS_H

#include <common/types.h>

/**
 * A structure for storing commandline arguments
 * passed into Raspboot
 *
 */
typedef struct
{
    /// Set to true if Raspboot should execute the uploaded binary
    /// as soon as the upload completes.
    U32     go;

    /// Set to true if Raspboot should terminate as soon as the binary
    /// is uploaded (and excuted, if -g is specified).
    U32     terminate;
    
    /// Set to true if Raspboot should print usage information. Only
    /// checked if no other arguments or serial port were specified.
    U32     help;

    /// Set to the desired verbosity level for Raspboot logging
    U32     verbosity;

    /// The location in the device memory to place the binary code.
    U32     location;

    /// The baud rate to use for the serial connection
    U32     speed;

    /// Filename for the binary to upload to the device
    char*   binary;

    /// Device name of the serial connection
    char*   port;
} Raspboot_Args;

/**
 * Initializes a Raspboot_Args instance to the default values
 *
 * @param   args    A pointer to the Raspboot_Args struct to initialize
 *
 * @return  0 on success, else an error code
 */
S32 raspbootArgsInit(Raspboot_Args* args);

/**
 * Cleans up a Raspboot_Args instance
 *
 * @param   args    A pointer to the Rasboot_Args struct to cleanup
 *
 * @return  0 on success, else and error code
 */
S32 raspbootArgsDestroy(Raspboot_Args* args);

/**
 * Parses input arguments
 *
 * @param   args    A pointer to a Raspboot_Args struct
 * @param   argc    The number of arguments to parse
 * @param   argv    The char* vector of arguments
 *
 * @return  0 on success, else an error code
 */
S32 raspbootArgsParse(Raspboot_Args* args, S32 argc, char** argv);

/**
 * Prints Raspboot usage information to stdout
 */
void raspbootArgsUsage();

#endif