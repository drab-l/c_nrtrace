#!/bin/sh
PP () {
    clang --target=${TARGET} --include=sys/syscall.h -E "$@"
}

TR () {
    grep "define [^ ]*_NR_"|cut -d" " -f2|xargs -I@ echo @ DDDD_@
}

NR () {
    grep DDDD_|sort -g|sed 's@DDDD_@'"${ARCH}"'@;s@\(.*\) \(.*\)@#define \2 \1@'
}

cd `dirname $0`
ARCH=aarch64 ; TARGET=aarch64-linux-gnu ; echo|PP -dM -|TR|PP -|NR > aarch64/NR.h
ARCH=arm ; TARGET=arm-linux-gnueabi ; echo|PP -dM -|TR|PP -|NR|grep -v -eARM_NR_BASE -eSYSCALL_BASE -eSYSCALL_MASK -earm_sync_file_range > arm/NR.h
ARCH=x86_64 ; TARGET=x86_64-linux-gnu ; echo|PP -dM -|TR|PP -|NR > x86_64/NR.h
ARCH=x86 ; TARGET=i386-linux-gnu ; echo|PP -dM -|TR|PP -|NR > x86/NR.h
echo unknown > syscall_list.h
cat */NR.h|cut -d" " -f2|grep -o _NR_.*|sed "s;^[^ ]*_NR_;;"|sort|uniq >> syscall_list.h
