#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_VERBOSITY_ERROR     0
#define CONSOLE_VERBOSITY_WARN      1
#define CONSOLE_VERBOSITY_INFO      2

#include "args.h"

/**
 * Prints a fatal rror message to the stderr and terminates Raspboot
 *
 * @param   args    A pointer to the Raspboot configuration
 * @param   fmt     The format string to print
 * @param   ...     Variadic arguments for the format string
 */
void raspbootConsoleFatal(Raspboot_Args* args, const char* fmt, ...);

/**
 * Prints an error message to the console
 *
 * @param   args    A pointer to the Raspboot configuration
 * @param   fmt     The format string to print
 * @param   ...     Variadic arguments for the format string
 */
void raspbootConsoleError(Raspboot_Args* args, const char* fmt, ...);

/**
 * Prints a warning message to the console
 *
 * @param   args    A pointer to the Raspboot configuration
 * @param   fmt     The format string to print
 * @param   ...     Variadic arguments for the format string
 */
void raspbootConsoleWarn(Raspboot_Args* args, const char* fmt, ...);

/**
 * Prints an info message to the console
 *
 * @param   args    A pointer to the Raspboot configuration
 * @param   fmt     The format string to print
 * @param   ...     Variadic arguments for the format string
 */
void raspbootConsoleInfo(Raspboot_Args* args, const char* fmt, ...);

#endif