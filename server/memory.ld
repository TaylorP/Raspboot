MEMORY
{
    ram : ORIGIN = 0x8000, LENGTH = 0x1000
}

SECTIONS
{
    .init : { *(.init*) } > ram
    .text : { *(.text*) } > ram
    .bss : { *(.bss*) } > ram
}