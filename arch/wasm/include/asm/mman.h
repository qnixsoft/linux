#ifndef WASM32_MMAN_H
#define WASM32_MMAN_H
#include <uapi/asm-generic/mman.h>
void *sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);
#endif
