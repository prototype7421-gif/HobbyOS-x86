ENTRY(start)
SECTIONS
{
    . = 0x200000;
    .multiboot : ALIGN (4)
    {
      *(.multiboot)
    }
    .text :
    {
       *(.text)
    }
    .data :
    {
        *(.data)
    }
    .bss : ALIGN (16)
    {
        *(.bss)
    }

}