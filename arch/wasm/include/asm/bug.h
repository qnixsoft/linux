/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Regents of the University of California
 */

#ifndef _ASM_RISCV_BUG_H
#define _ASM_RISCV_BUG_H

#include <linux/compiler.h>
#include <linux/const.h>
#include <linux/types.h>

#include <asm/asm.h>

#define __INSN_LENGTH_MASK  _UL(0x3)
#define __INSN_LENGTH_32    _UL(0x3)
#define __COMPRESSED_INSN_MASK	_UL(0xffff)

#define __BUG_INSN_32	_UL(0x00100073) /* ebreak */
#define __BUG_INSN_16	_UL(0x9002) /* c.ebreak */

#define GET_INSN_LENGTH(insn)						\
({									\
	unsigned long __len;						\
	__len = ((insn & __INSN_LENGTH_MASK) == __INSN_LENGTH_32) ?	\
		4UL : 2UL;						\
	__len;								\
})

typedef u32 bug_insn_t;

#define __BUG_FLAGS(flags) do {					\
	ASM_VOLATILE("ebreak\n");			\
} while (0)

#define BUG()	do {								\
	pr_warn("BUG: failure at %s:%d/%s()!\n", __FILE__, __LINE__, __func__); \
	barrier_before_unreachable();						\
	__builtin_trap();							\
} while (0)
	/* __BUG_FLAGS(0);						\ */
	/* unreachable();						\ */

#define __WARN_FLAGS(flags) __BUG_FLAGS(BUGFLAG_WARNING|(flags))

#define HAVE_ARCH_BUG

#include <asm-generic/bug.h>

struct pt_regs;
struct task_struct;

void die(struct pt_regs *regs, const char *str);
void do_trap(struct pt_regs *regs, int signo, int code, unsigned long addr);

#endif /* _ASM_RISCV_BUG_H */
