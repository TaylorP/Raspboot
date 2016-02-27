#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "console.h"

void raspbootConsoleFatal(Raspboot_Args* args, const char* fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    fprintf(stderr, "Fatal error: ");
    vfprintf(stderr, fmt, argp);
    fprintf(stderr, "\n");
    va_end(argp);

    exit(1);
}

void raspbootConsoleError(Raspboot_Args* args, const char* fmt, ...)
{
    if (args == 0 || args->verbosity >= CONSOLE_VERBOSITY_ERROR)
    {
        va_list argp;
        va_start(argp, fmt);
        fprintf(stderr, "Error: ");
        vfprintf(stderr, fmt, argp);
        fprintf(stderr, "\n");
        va_end(argp);
    }
}

void raspbootConsoleWarn(Raspboot_Args* args, const char* fmt, ...)
{
    if (args != 0 && args->verbosity >= CONSOLE_VERBOSITY_WARN)
    {
        va_list argp;
        va_start(argp, fmt);
        fprintf(stdout, "Warning: ");
        vfprintf(stdout, fmt, argp);
        fprintf(stdout, "\n");
        va_end(argp);
    }
}

void raspbootConsoleInfo(Raspboot_Args* args, const char* fmt, ...)
{
    if (args != 0 && args->verbosity >= CONSOLE_VERBOSITY_INFO)
    {
        va_list argp;
        va_start(argp, fmt);
        vfprintf(stdout, fmt, argp);
        fprintf(stdout, "\n");
        va_end(argp);
    }
}