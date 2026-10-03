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