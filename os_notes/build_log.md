# setting up toolchains and QEMU
cd /mnt/d/VSCODE/OS
>pwd

gcc --version
as --version
ld --version
make --version

qemu-system-x86_64 --version
nasm -v

i686-elf-gcc --version
x86_64-elf-gcc --version

grub-file --version

sudo apt update
sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev
>QEMU
sudo apt install qemu-system-x86

AFTER AN OBJECT ASSEMBLY FILE IS CREATED DISSECT 
>file hello
file hello.o- tells type of file
readelf -h hello= tells the architecture - 64 or 32
readelf -h hello.o-
readelf -s hello.o- symbols of object file
# OUR ACTUAL OS PIPELINE-:
Assembly/C source
       ↓
bare-metal toolchain
       ↓
object files
       ↓
linker + linker script
       ↓
kernel image
       ↓
bootloader / firmware path
       ↓
QEMU
       ↓
CPU
       ↓
YOUR KERNEL

> Binutils(Linker tools) source download
export TARGET=i686-elf
export PREFIX="$HOME/opt/cross"
export PATH="$PREFIX/bin:$PATH"
>download
cd ~/src
wget https://sourceware.org/pub/binutils/releases/binutils-2.47.tar.xz
tar -xf binutils-2.47.tar.xz

> make a directory
cd ~/src
mkdir -p build-binutils
cd build-binutils
> configure bintuils
../binutils-2.47/configure \
  --target=$TARGET \
  --prefix="$PREFIX" \
  --with-sysroot \
  --disable-nls \
  --disable-werror

>build bintuils
make -j$(nproc)

>install
make install

# GCC source download
cd ~/src
wget https://gcc.gnu.org/pub/gcc/releases/gcc-16.2.0/gcc-16.2.0.tar.xz
tar -xf gcc-16.2.0.tar.xz
> build gcc
make all-gcc -j$(nproc)
>install
make install-gcc
>build lib gcc
make all-target-libgcc -j$(nproc)
>install
$PREFIX/bin/i686-elf-gcc --version

> set path permanent
echo 'export PATH="$HOME/opt/cross/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
# setting up GRUB (bootloader)
sudo apt update
sudo apt install grub2-common xorriso
# DATE - 3/10/26
. Our Current Project Structure

Current project directory:

/mnt/d/VSCODE/OS

Current structure:

OS/
├── boot/
│   └── boot.s
├── kernel/
├── hello
├── hello.asm
├── hello.o
└── os_notes/
> created instruction for multiboot header
>also reserved 16bit memory for stack
> pointed the stack pointer towards stack_top.
# 4/10/26
finally loaded the kernel through kernel still cant type though...
boot.asm
   ↓
Assembler
   ↓
boot.o
   ↓
Linker + linker.ld
   ↓
kernel.elf
   ↓
GRUB
   ↓
Bootable ISO
   ↓
QEMU
   ↓
GNU GRUB menu

Our bootstrap Assembly currently handles:

Multiboot v1 header
Stack reservation
Stack pointer setup
Kernel entry point
Halting the CPU after startup
>linker script 
used it well basically its job its to tell -
where the kernel starts
which symbol is the entry point
how sections are arranged

>problems faced
our  multiboot header was not taken as the part of the kernel image
so we changed its flag then it was allocatable to the loader.

second prblm was qemu failed to regognize or boot our OS
so we used
sudo apt install grub-pc-bin
this command now broadend it search for our kernel  image.
final project folder for today
OS/
├── boot/
├── kernel/
├── linker/
├── kernel.elf
└── isodir/
    └── boot/
        └── grub/
            └── grub.cfg


1. Why can't we use normal main() as our kernel entry?
2. Why do we need kernel_main()?
3. Why is a valid stack required before calling C?
4. Why are boot.o and kernel.o linked together?
5. What does -ffreestanding mean?

> its because its a C function tht will be compiled by the OS we are using but tachnically we dont have an os so we want our C code to not recognize windows or linux.basically we have to make our own entry point.

>stack is needed because it is part of our memory our c program should be stored in a stack of our kernel means then only it will implement changes in our kernel.

> those obbject files are linked together by linker because thts how our machine will understand the program the linker dont know our OS and kernel entry point..

> ffreestanding - there is no OS under our kernel so if we execute C program withou this it will think there is an OS runtime environment like window and linux.

# 6/10/26
1. Kernel boot flow understood

Understood the basic flow:

GRUB
 ↓
boot.asm
 ↓
kernel_entry()
 ↓
kernel.c
 ↓
VGA memory
 ↓
Screen output
2. Kernel C became freestanding

Learned that kernel code cannot depend on normal hosted C libraries like:
stdio.h
stdlib.h
So the kernel entry function is kept simple and freestanding.

3. VGA Text Mode

Learned that screen output can be written directly to:
0xB8000
This is VGA text-mode video memory.
Each screen position uses 16 bits:

[ 8-bit attribute ][ 8-bit character ]

For example:
0x0F48
means the character H with the selected text attribute.

4. Pointers and array-style access

Understood that a pointer can be accessed using:
ptr[i]
which is conceptually:
*(ptr + i)
Also learned that the pointer type determines how far i moves in memory.
>problems faces- QEMU wasnt loading turns out i wrote the code wrong ! 

