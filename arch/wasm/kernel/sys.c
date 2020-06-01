/* #include <linux/unistd.h> */
/* #include <syscall.h> */
#include <asm/syscall.h>
#include <asm/syscalls.h>
#include <linux/syscalls.h>
#include <asm/linkage.h> /* __wasm_export */
#include <asm/ar.h>
#include <asm/syscall_wrapper.h>
#include <linux/printk.h>
#include <linux/mm.h>

#define AR_FB(ar, ...) AR_FB##ar(__VA_ARGS__)
#define AR_FB0()
#define AR_FB1(t, n) , (unsigned long)n
#define AR_FB2(t, n, ...) , (unsigned long)n AR_FB1(__VA_ARGS__)
#define AR_FB3(t, n, ...) , (unsigned long)n AR_FB2(__VA_ARGS__)
#define AR_FB4(t, n, ...) , (unsigned long)n AR_FB3(__VA_ARGS__)
#define AR_FB5(t, n, ...) , (unsigned long)n AR_FB4(__VA_ARGS__)
#define AR_FB6(t, n, ...) , (unsigned long)n AR_FB5(__VA_ARGS__)

#define AR_LF0
#define AR_LF1 "%lu"
#define AR_LF2 AR_LF1 "," AR_LF1
#define AR_LF3 AR_LF2 "," AR_LF1
#define AR_LF4 AR_LF3 "," AR_LF1
#define AR_LF5 AR_LF4 "," AR_LF1
#define AR_LF6 AR_LF5 "," AR_LF1

#define SYSCALL_DEBUG(ar, name, ret, ...) \
	METAL_SYSCALL_DEFINE(ar, name, __VA_ARGS__) { \
		pr_err(#name "(d)[%d] ("AR_LF##ar")\n", \
		       ar AR_FB(ar, __VA_ARGS__)); \
		return ret; \
	}

#define SYSCALL_LDEBUG(ar, name, ret) SYSCALL_DEBUG(ar, name, ret, AR_LGEN##ar)

#define METAL_SYSCALL_DECLARE(ar, name, ...) \
        long sys_m_##name(__VA_ARGS__);

#include <uapi/linux/uio.h>

/* SYSCALL_DEBUG(1, exit, a, int, a); */
/* SYSCALL_DEBUG(3, ioctl, 0, int, fd, int, req, void *, arg); */

#undef __SYSCALL
#define __SYSCALL(nr, call) [nr] = (sys_call_ptr_t) (call),

asmlinkage const sys_call_ptr_t sys_call_table[__NR_syscalls+1] = {
	[ 0 ... NR_syscalls - 1] = sys_ni_syscall,
#include <asm/unistd.h>
};

/* long sys_ni_syscall(void); */

#define DEFSYSCALL(ar) \
	asmlinkage __visible __wasm_export long __init __syscall##ar(long n AR_DECL(ar, AR_LGEN##ar)) { \
		if (*sys_call_table[n] == &sys_ni_syscall) { \
			pr_err("ni_syscall[%d][%ld] ("AR_LF##ar")\n", \
				   ar, n AR_FB(ar, AR_LGEN##ar)); \
			return sys_ni_syscall(); \
		} \
		return (* sys_call_table[n])(AR_PASS_(ar, AR_LGEN##ar)); \
	}

DEFSYSCALL(0);
DEFSYSCALL(1);
DEFSYSCALL(2);
DEFSYSCALL(3);
DEFSYSCALL(4);
DEFSYSCALL(5);
DEFSYSCALL(6);

SYSCALL_DEFINE6(mmap, unsigned long, addr, unsigned long, len,
		unsigned long, prot, unsigned long, flags,
		unsigned long, fd, off_t, off)
{
	if (offset_in_page(off) != 0)
		return -EINVAL;

	return ksys_mmap_pgoff(addr, len, prot, flags, fd, off >> PAGE_SHIFT);
}

SYSCALL_DEFINE5(clone,
		unsigned long, a,
		unsigned long, b,
		int __user *, c,
		unsigned long, d,
		int __user *, e)
{
	return 1;
}

/* int nanosleep(const struct timespec *timeout, struct timespec *remainder); */

SYSCALL_DEFINE1(adjtimex,
		struct __kernel_timex __user *, txc_p)
{
	return 1;
}


SYSCALL_DEFINE2(nanosleep,
		struct __kernel_timespec __user *, timeout,
		struct __kernel_timespec __user *, remainder)
{
	return 1;
}

int __switch_to(int a, int b) {
	return 0;
}
