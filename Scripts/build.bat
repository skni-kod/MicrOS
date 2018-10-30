rem Build script for MicrOS. Compiles bootloader, kernel, and uploads them to the floppy.

rem Compile bootloader
Tools\nasm.exe Source\Bootloader\bootloader.asm -f bin -o Build\bootloader.bin

rem Check if bootloader has been compiled with success
if not exist Build\bootloader.bin (
    echo "Bootloader error!"
    exit
)

rem Compile kernel
Tools\nasm.exe Source\Kernel\kernel.asm -f bin -o Build\kernel.bin

rem Check if kernel has been compiled with success
if not exist Build\kernel.bin (
    echo "Kernel error!"
    exit
)

rem Upload bootloader to the floppy
Tools\dd.exe if=Build\bootloader.bin bs=512 of=Build\floppy.img

rem Mount floppy
imdisk -a -f Build\floppy.img -m F:

rem Copy kernel to the floppy
copy Build\kernel.bin F:\kernel.bin

rem Unmount floppy
imdisk -D -m F: