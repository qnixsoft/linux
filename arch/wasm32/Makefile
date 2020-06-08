# CROSS_COMPILE=/usr/local/bin/clang

# cflags-y	+= -fno-common -pipe -fno-builtin -mmedium-calls -D__linux__
# cflags-y	+= -fno-common -pipe -fno-builtin -mmedium-calls -D__linux__

core-y	+= arch/wasm32/kernel/
libs-y	+= arch/wasm32/lib/
core-y	+= arch/wasm32/mm/

DESTDIR ?= /
PREFIX ?= ${DESTDIR}/usr/local
MKDIR ?= ${PREFIX}/mk
include ${MKDIR}/wasm.mk
