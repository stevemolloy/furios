#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/furios.kernel isodir/boot/furios.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "furios" {
	multiboot /boot/furios.kernel
}
EOF
grub-mkrescue -o furios.iso isodir
