# MicrOS
[![Build Status](https://i.imgur.com/lNuuSvc.png)](https://travis-ci.org/Tearth/MicrOS)

<p align="center">
<a href="https://travis-ci.org/Tearth/MicrOS"><img src="https://travis-ci.org/Tearth/MicrOS.svg?branch=develop" alt="Build status"></img></a>
<a href="https://github.com/Tearth/MicrOS/pulls?q=is%3Apr+is%3Aclosed"><img src="https://img.shields.io/github/issues-pr-closed-raw/Tearth/MicrOS" alt="Closed pull requests"></img></a>
<a href="https://github.com/Tearth/MicrOS/blob/master/LICENSE"><img src="https://img.shields.io/github/license/Tearth/MicrOS" alt="License"></img></a>
<a href="https://tearth.github.io/MicrOS/"><img src="https://img.shields.io/badge/Doxygen-gh--pages-blue" alt="Documentation"></img></a>
</p>

This is the simple 32-bit operating system developed by the SKNI KOD (Studenckie Koło Naukowe Informatyków KOD). Our main goal is to create the OS which will have functionality similar to the famous MS-DOS and be able to run on every modern computer.

The repository doesn't contain all files necessary to successful compilation - we use our internal build scripts which aren't present here. We will release complete image files when the OS will be ready for basic usage. Feel free to analyze and use any part of this project (but remember about GPL-3 license restrictions).

Full description about how to build MicrOS (on Linux or Windows 10 WSL) can be read here: https://github.com/jaenek/MicrOS-DevTools.

Documentation: https://tearth.github.io/MicrOS/

## What is done
* Bootloader which loads the kernel by looking for a file in a FAT12 filesystem and enters the protected mode
* A set of drivers for the devices
  * VGA - text and graphics mode
  * Keyboard
  * Floppy
  * PC Speaker
  * RTC (Real-Time Clock)
  * PIT (Programmable Interval Timer)
* Interrupts (software, hardware, exceptions) manager
* Paging
* Physical/Virtual memory manager
* Heap
* FAT12/16 filesystem - read and write
* Partitions
* PCI (Peripheral Component Interconnect) - partially
* Panic screen
* User space
* ELF executing
* Multitasking (with primitive round-rubin scheduler)
* Simple shell and basic applications
* Standard library for C
* ATA Hard disk drive support

## TODO
* Other standards of hard disk drive support
* More advanced scheduler with priorities and IO blocking
* Support for network cards (and protocols related to them)
* ...

## Used tools
* Visual Studio Code - main IDE
* GCC - C compiler and linker
* NASM - asm compiler
* GDB - debugger
* ImDisk - managing floppy images
* dd - writing bootloader to the image

## Examples
![MicrOS boot sequence](https://i.imgur.com/Y200tKQ.png)

Boot sequence, floppy calibration, reading content of the example file and heap dump.

![MicrOS multitasking](https://i.imgur.com/tKVWytt.png)

Multitasking with five processes incrementing internal counters and printing them on the console.

![Tasks manager](https://i.imgur.com/GETPuxo.png)

Tasks manager displaying basic info about the current operating system state.

![Shell](https://i.imgur.com/CpRxnFe.png)

Simple shell which allows to execute other applications.

![CAT application](https://i.imgur.com/CC2bwX6.png)

Example of working "cat" application.

![Snake game](https://i.imgur.com/8Bjz6IN.png)

Snake game.