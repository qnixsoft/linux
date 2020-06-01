# CROSS_COMPILE=/usr/local/bin/clang

# cflags-y	+= -fno-common -pipe -fno-builtin -mmedium-calls -D__linux__
# cflags-y	+= -fno-common -pipe -fno-builtin -mmedium-calls -D__linux__

include ${srctree}/../mk/wasm.mk

core-y	+= arch/${ARCH}/kernel/
libs-y	+= arch/${ARCH}/lib/
core-y	+= arch/${ARCH}/mm/
