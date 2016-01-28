.section .init

.globl _start
_start:
    msr cpsr_c, #0xD3
    mov sp,#0x8000000
    bl main

.globl _enterAddress
_enterAddress:
    stmfd sp!, {r0-r12, lr} 
    mov lr, pc
    mov pc, r0
    ldmfd sp!, {r0-r12, lr} 
    bx lr
