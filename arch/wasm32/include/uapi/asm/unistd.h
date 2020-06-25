#if !defined(ARCH_WASM32_UNISTD_H) || defined(__SYSCALL)
#define ARCH_WASM32_UNISTD_H

#ifndef __SYSCALL
#define __SYSCALL(x, y)
#endif

#include <uapi/asm-generic/unistd.h>

#undef __NR_syscalls
#define __NR_syscalls 294

#undef __SYSCALL

#endif /* !defined(ARCH_WASM32_UNISTD_H) || defined(__SYSCALL) */
