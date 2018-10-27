imdisk -a -f Build\floppy.img -m F:
copy Build\kernel.bin F:\kernel.bin
imdisk -D -m F: