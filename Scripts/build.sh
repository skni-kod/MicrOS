#!/bin/bash

# Compile bootloader
Tools/nasm.exe Source/Bootloader/bootloader.asm -f bin -o Build/bootloader.bin

if [ ! -f Build/bootloader.bin ]; then
    echo "Bootloader build error!"
    exit
fi

# Compile kernel startup
Tools/nasm.exe Source/Kernel/startup.asm -f elf32 -o Build/Kernel/startup.o

if [ ! -f Build/Kernel/startup.o ]; then
    echo "Kernel startup build error!"
    exit
fi

# Go to directory where all kernel output files will be stored
cd Build/Kernel

# Find all source files with *.c extension
SourceFiles=`find ../.. -name '*.c'`

# Compile kernel
~/opt/cross/bin/i386-elf-gcc -g -c $SourceFiles -ffreestanding -Wall -Wextra

if [ $? -ne 0 ]; then
    echo "Kernel build error!"
    exit
fi

# Back to the main project directory
cd ../..

# Find all output files with *.o extension
OutputFiles=`find . -name '*.o'`

# Link kernel output files
~/opt/cross/bin/i386-elf-gcc -g -T Source/linker.ld -o Build/kernel.elf -ffreestanding -O2 -nostdlib $OutputFiles -lgcc

# Get debug symbols from elf and save them to the separate file
objcopy --only-keep-debug Build/kernel.elf Build/kernel.sym

# Get flat binary from elf file and save it as our real kernel file
objcopy -O binary Build/kernel.elf Build/kernel.bin

if [ $? -ne 0 ]; then
    echo "Linker error!"
    exit
fi

# Upload bootloader to the floppy
Tools/dd.exe if=Build/bootloader.bin bs=512 of=Build/floppy.img

# Mount floppy
imdisk -a -f Build/floppy.img -m F:

# Copy kernel to the floppy
cp Build/kernel.bin F:/kernel.bin

# Unmount floppy
imdisk -D -m F: