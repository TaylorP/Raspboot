.section .init

.globl _start
_start:
    msr cpsr_c, #0xD3
    mov sp,#0x8000000
    bl main
