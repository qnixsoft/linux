#include <linux/syscalls.h>

SYSCALL_DEFINE0(rt_sigreturn)
{
	return 0;
}
