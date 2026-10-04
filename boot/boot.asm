.section .multiboot,"a"
.align 4

.long 0x1BADB002
.long 3
.long 0xE4524FFB


.section .bss
.align 16

stack_bottom:
    .skip 16384

stack_top:


.section .text
.globl start
.type start, @function

start:
    movl $stack_top, %esp

1:
    cli
    hlt
    jmp 1b

.size start, . - start
