#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/scottos.kernel isodir/boot/scottos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "scottos" {
  multiboot /boot/scottos.kernel
}
EOF
grub-mkrescue -o scottos.iso isodir
