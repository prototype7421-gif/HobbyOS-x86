 # OS Basics — My Operating System Development Notes

1. Our Goal

The goal of this project is to build a small operating system from scratch for learning.

We are developing and testing it safely inside Windows using WSL + QEMU.

We are not replacing Windows.

The long-term project can grow toward:

Kernel development

Memory management

Interrupts

Processes and scheduling

Drivers

Filesystems

User programs

Eventually a basic GUI

For now, the goal is much smaller:

Boot our own kernel and make the CPU execute our code.

2. Basic Boot Flow

When a computer starts, the simplified flow is:

Power On
   ↓
Firmware (UEFI/BIOS)
   ↓
Boot target
   ↓
Bootloader
   ↓
Kernel
   ↓
Operating System

Important distinction:

Firmware initializes the machine and starts the boot process.

Bootloader loads the operating system kernel.

Kernel is the core of the operating system.

For our project, GRUB will act as the bootloader.

3. Our Development Environment

The development setup is:

Windows
   ↓
WSL Ubuntu
   ↓
Cross Compiler / Binutils
   ↓
Our Kernel
   ↓
GRUB
   ↓
QEMU
   ↓
Virtual Machine

QEMU gives us virtual hardware, so a kernel crash should not damage the Windows installation.

4. CPU Basics

The CPU executes machine instructions.

The simplified instruction cycle is:

Fetch
  ↓
Decode
  ↓
Execute

The CPU repeatedly reads instructions, understands them, and executes them.

5. CPU Registers

Registers are very small, very fast storage locations inside the CPU.

Important registers discussed so far:

General-purpose registers

Examples:

RAX
RBX
RCX
RDX

These can be used for temporary data and calculations.

Instruction Pointer

RIP

RIP keeps track of where the next instruction comes from in x86-64 execution.

Stack Pointer

RSP

RSP points to the current top of the stack.

Note: Our first Bare Bones kernel uses the 32-bit i686 target, so the exact register names in the kernel's Assembly will be 32-bit equivalents rather than relying on x86-64 registers everywhere.

6. RAM vs Registers

Registers are inside the CPU.

RAM is separate system memory.

CPU
├── Registers
└── Execution logic

        ↕
      RAM

Registers are extremely fast but limited in size.

RAM is much larger and is used for programs, data, stacks, etc.

7. The Stack

The stack is a region of memory used for temporary information.

It follows the:

LIFO
Last In, First Out

idea.

The stack is commonly involved in:

Function calls

Return information

Local variables

Saved register values

The stack pointer keeps track of the current top of the stack.

Conceptually:

Function A
   ↓
Function B
   ↓
Function C

Each function call may use stack space.

For our kernel:

GRUB
   ↓
boot entry
   ↓
set up stack
   ↓
call kernel_main()

A valid stack is important before safely handing execution to C code.

8. Privilege Levels

A modern operating system separates code into different privilege levels.

A simplified x86 view is:

Ring 0 → Kernel
Ring 3 → User applications

Kernel code has much more privilege than normal applications.

User programs should not be allowed to directly perform every privileged operation.

The kernel acts as the controlled middle layer between applications and hardware/resources.

9. CPU Modes

We discussed the major x86 execution modes:

Real Mode

An old/basic 16-bit execution mode.

Protected Mode

Provides 32-bit protected execution and memory protection features.

Long Mode

The 64-bit execution mode used by modern x86-64 operating systems.

Our first learning milestone uses an i686-elf 32-bit target because the classic Bare Bones path lets us learn booting and kernel fundamentals in a smaller step.

This does not mean modern operating systems must remain 32-bit.

10. Assembly

Assembly is a human-readable representation of low-level CPU instructions.

The CPU ultimately executes machine code.

A simplified relationship is:

Assembly source
      ↓
Assembler
      ↓
Machine-code object file

Important Assembly concepts:

Registers

Moving data

Memory access

Arithmetic

Compare

Jumps

Calls

Returns

Assembly is especially useful during early kernel startup because we need direct control over CPU-specific details.

11. C and Assembly in the Kernel

The general idea is:

Assembly
   ↓
Early CPU setup
   ↓
C kernel
   ↓
Higher-level kernel logic

Assembly is useful for very low-level startup and CPU-specific work.

C is useful for most kernel logic because it is much easier to manage large amounts of code.

12. Normal GCC vs Cross Compiler

A normal compiler such as:

gcc

is intended to build programs for the normal host environment.

Our kernel is not a normal Linux application.

Therefore we use a cross compiler:

i686-elf-gcc

Meaning:

i686
  ↓
32-bit x86 target

elf
  ↓
ELF object/binary format

The important idea is:

Host:
WSL Ubuntu

Target:
our future OS

The compiler must build code for the target environment rather than assuming Linux is providing the normal runtime.

13. Toolchain

A toolchain is the collection of tools used to turn source code into the final kernel image.

Important tools in our setup include:

i686-elf-gcc
i686-elf-as
i686-elf-ld

i686-elf-gcc

Compiles C for our target.

i686-elf-as

Assembler for the target architecture.

i686-elf-ld

Linker that combines object files into the final linked image.

General idea:

C source
   ↓
Compiler
   ↓
C object

Assembly source
   ↓
Assembler
   ↓
Assembly object

C object + Assembly object + other objects
   ↓
Linker
   ↓
Kernel image

14. Linker

The linker combines separately compiled pieces into one final executable/image.

It also decides how the different sections of the program are arranged.

This is why we will use a linker script:

linker.ld

A linker script is basically a memory/layout plan for the linker.

Simple analogy:

Compiler/assembler create the building pieces.
The linker puts those pieces together.
The linker script acts like the floor plan.

15. ELF

ELF stands for:

Executable and Linkable Format

It is a common format for executable files and object files on Unix-like systems.

We inspected our earlier Assembly experiment:

hello
hello.o

and saw ELF headers.

Important distinction:

hello.o

was a relocatable object file.

hello

was a linked executable for Linux.

That experiment was useful for understanding the toolchain, but it was not our OS kernel.

16. GRUB

GRUB is our bootloader.

Its job in this project is to:

Load the kernel.

Recognize the kernel as a supported Multiboot kernel.

Transfer control to the kernel's entry point.

Conceptually:

Firmware
   ↓
GRUB
   ↓
Our kernel

GRUB saves us from having to write a complete bootloader ourselves at the beginning.

17. Multiboot

Our first kernel follows the Multiboot v1 Bare Bones approach.

GRUB needs a recognizable Multiboot header inside the kernel image.

The header tells GRUB:

This kernel follows the Multiboot format.

For Multiboot v1, the important basic fields we discussed are:

magic
flags
checksum

18. Multiboot Magic

The standard Multiboot v1 magic value is:

0x1BADB002

This is the fixed identifier GRUB looks for when checking for a Multiboot header.

The header must also be placed where GRUB can find it:

Within the first 8192 bytes of the kernel image

4-byte aligned

19. Multiboot Flags

The flags field describes information/features requested by the kernel.

For the common Bare Bones starting point, the commonly used value is:

3

This enables:

bit 0 → request page-aligned loaded modules
bit 1 → request memory information

The exact meaning comes from the Multiboot v1 specification.

20. Multiboot Checksum

The Multiboot v1 checksum must satisfy:

magic + flags + checksum = 0

using 32-bit arithmetic.

The purpose is to let GRUB verify that the header values are internally consistent.

Conceptually:

magic
  +
flags
  +
checksum
  =
0

21. Kernel Entry Point

After GRUB loads the kernel, execution must begin at a defined entry point.

Our early kernel structure will be:

GRUB
  ↓
Multiboot header recognized
  ↓
kernel entry point
  ↓
initial Assembly setup
  ↓
stack setup
  ↓
kernel_main()
  ↓
kernel logic

22. Why boot.s Exists

Our project currently has:

boot/
└── boot.s

boot.s is not the complete bootloader.

GRUB is already doing the bootloader work.

Our Assembly file is the low-level bridge between GRUB and the C part of our kernel.

Its early responsibilities will include:

Providing the Multiboot header.

Providing the entry point.

Setting up the stack.

Passing control to the C kernel.

23. Why We Need a Stack Before C

C function calls depend on a valid stack.

Our intended flow is:

GRUB
  ↓
boot.s entry
  ↓
stack setup
  ↓
call kernel_main()
  ↓
kernel.c

So the Assembly startup code prepares the CPU environment before normal C kernel logic begins.

26. QEMU

QEMU provides a virtual machine/emulated hardware environment.

Instead of immediately trying our kernel on physical hardware:

Our kernel
   ↓
QEMU
   ↓
Virtual CPU / Memory / Devices

This gives us a much safer development and debugging environment.

 # 27. Project Build Flow

Our intended kernel build pipeline is:

kernel.c
     ↓
i686-elf-gcc
     ↓
kernel.o

boot.s
     ↓
Assembler
     ↓
boot.o

boot.o + kernel.o
     ↓
i686-elf-ld
     ↓
kernel image

kernel image
     ↓
GRUB
     ↓
Bootable ISO
     ↓
QEMU
     ↓
Our OS

29. Important Development Rule

We are following a learning-first workflow:

Understand
   ↓
Attempt ourselves
   ↓
Build
   ↓
Validate
   ↓
Run in QEMU
   ↓
Debug

The goal is not to copy a finished kernel.

The goal is to understand what each part does and eventually be able to build it independently.

30. Mental Model So Far

The entire project can currently be visualized as:

Windows
  ↓
WSL Ubuntu
  ↓
Cross Toolchain
  ↓
Build our kernel
  ↓
GRUB
  ↓
Multiboot validation
  ↓
Assembly entry
  ↓
Stack
  ↓
C kernel
  ↓
QEMU virtual hardware
  ↓
Our OS
>STACK SETUP
So after the entry point is given we need to setup some space for stack why?
because to store and execute instruction we need on!!!
Higher addresses
        ↓

0x2000   ← old/high address
0x1FFC
0x1FF8
0x1FF4
        ↑
      stack grows this way
        ↓

Lower addresses
> now reserving memory for stack
ok we ue 
.data- initialized data
.bss-it gives us the required space for stack(uninitialized data).
.text- it stores instruction and code.