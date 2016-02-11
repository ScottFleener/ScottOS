#!/bin/bash
i686-elf-as boot.s -o boot.o
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T linker.ld -o scottos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
cp scottos.bin isodir/boot/
grub-mkrescue -o scottos.iso isodir
