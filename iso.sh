#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/dios.kernel isodir/boot/dios.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "dios" {
  multiboot /boot/dios.kernel
}
EOF
grub-mkrescue -o dios.iso isodir
