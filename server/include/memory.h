#ifndef MEMORY_H
#define MEMORY_H

#include <common/types.h>

/**
 * Stores a U32 value to a memory location
 *
 * @param   dst     The memory address to write to
 * @param   src     The value to write
 *
 * @return  no return value
 */
static inline void __str(U32 dst, U32 src)
{
    asm volatile("str %0, [%1]" : : "r" (src), "r" (dst) : "memory");
}

/**
 * Stores a U8 value to a memory location
 *
 * @param   dst     The memory address to write to
 * @param   src     The value to write
 *
 * @return  no return value
 */
static inline void __strb(U32 dst, U8 src)
{
    asm volatile("strb %0, [%1]" : : "r" (src), "r" (dst) : "memory");
}

/**
 * Loads a U32 value from a memory location
 *
 * @param   src     The memory address to read from
 *
 * @return  The value at the memory address
 */
static inline U32 __ldr(U32 src)
{
    asm volatile("ldr %0, [%1]" : "=r" (src) :  "r" (src) : "memory");
    return src;
}

/**
 * Loads a U8 value from a memory location
 *
 * @param   src     The memory address to read from
 *
 * @return  The value at the memory address
 */
static inline U32 __ldrb(U32 src)
{
    asm volatile("ldrb %0, [%1]" : "=r" (src) :  "r" (src) : "memory");
    return src;
}

#endif