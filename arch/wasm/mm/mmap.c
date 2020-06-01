// SPDX-License-Identifier: GPL-2.0-only
/*
 * ARC700 mmap
 *
 * (started from arm version - for VIPT alias handling)
 *
 * Copyright (C) 2013 Synopsys, Inc. (www.synopsys.com)
 */

#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/sched/mm.h>

#include <asm/cacheflush.h>

#define COLOUR_ALIGN(addr, pgoff)			\
	((((addr) + SHMLBA - 1) & ~(SHMLBA - 1)) +	\
	 (((pgoff) << PAGE_SHIFT) & (SHMLBA - 1)))

/*
 * Ensure that shared mappings are correctly aligned to
 * avoid aliasing issues with VIPT caches.
 * We need to ensure that
 * a specific page of an object is always mapped at a multiple of
 * SHMLBA bytes.
 */
unsigned long
arch_get_unmapped_area(struct file *filp, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags)
{
	pr_err("arch_get_unmapped_area\n");
	return 1;
}
