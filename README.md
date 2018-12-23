# MicrOS

This is the simple 32-bit operating system developed by the SKNI KOD (Studenckie Koło Naukowe Informatyków KOD). Our main goal is to create the OS which will have functionality similar to the famous MS-DOS and be able to run on every modern computer (but no, we won't sell it and compete with Windows or Linux).

The repository doesn't contain all files necessary to successful compilation - we use our internal build scripts which aren't present here. We will release complete image files when the OS will be ready to basic usage. Feel free to analyse and use any part of this project (but remember about GPL-3 license restrictions).

## What is done
* Bootloader which loads the kernel by looking for a file in a FAT12 filesystem and enters the protected mode
* A set of drivers for the devices
  * VGA - text and graphics mode
  * Keyboard
  * Floppy
  * PC Speaker
  * RTC (Real-Time Clock)
  * PIT (Programmable Interval Timer)
* Interrupts (software, hardware and exceptions) manager
* Paging
* Physical/Virtual memory manager
* Heap
* FAT12 filesystem - partially, currently read-only
* PCI (Peripheral Component Interconnect) - partially
* Panic screen (because the OS can't be named as OS if there is no beautiful panic screen on a page fault exception)
* User space
* ELF executing
* Multitasking (with primitive round-rubin scheduler, but it will be changed for something better)
* Standard library for C - partially

## TODO
* Full support for the FAT12, FAT16 and FAT32 filesystems
* Hard disk drive support
* Shell and other important applications
* More advanced scheduler with priorites and IO blocking
* Rest of the C standard library functions

## TODO (?)
* Abstraction layer between OS and devices
* Support for network cards (and protocols related with them)
* ...

## Used tools
* Visual Studio Code - main IDE
* GCC - C compiler and linker
* NASM - asm compiler
* GDB - debugger
* ImDisk - managing floppy images
* dd - writing bootloader to the image

## Example
![MicrOS example screenshot](https://i.imgur.com/Y200tKQ.png)

Boot sequence, floppy calibration, reading content of the example file and heap dump.