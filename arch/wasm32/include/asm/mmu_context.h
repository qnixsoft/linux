/*
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * vineetg: May 2011
 *  -Refactored get_new_mmu_context( ) to only handle live-mm.
 *   retiring-mm handled in other hooks
 *
 * Vineetg: March 25th, 2008: Bug #92690
 *  -Major rewrite of Core ASID allocation routine get_new_mmu_context
 *
 * Amit Bhor, Sameer Dhavale: Codito Technologies 2004
 */

#ifndef _ASM_ARC_MMU_CONTEXT_H
#define _ASM_ARC_MMU_CONTEXT_H

#include <asm/arcregs.h>
#include <asm/tlb.h>
#include <linux/sched/mm.h>

#include <asm-generic/mm_hooks.h>
#include <asm-generic/mmu_context.h>

/*		ARC700 ASID Management
 *
 * ARC MMU provides 8-bit ASID (0..255) to TAG TLB entries, allowing entries
 * with same vaddr (different tasks) to co-exit. This provides for
 * "Fast Context Switch" i.e. no TLB flush on ctxt-switch
 *
 * Linux assigns each task a unique ASID. A simple round-robin allocation
 * of H/w ASID is done using software tracker @asid_cpu.
 * When it reaches max 255, the allocation cycle starts afresh by flushing
 * the entire TLB and wrapping ASID back to zero.
 *
 * A new allocation cycle, post rollover, could potentially reassign an ASID
 * to a different task. Thus the rule is to refresh the ASID in a new cycle.
 * The 32 bit @asid_cpu (and mm->asid) have 8 bits MMU PID and rest 24 bits
 * serve as cycle/generation indicator and natural 32 bit unsigned math
 * automagically increments the generation when lower 8 bits rollover.
 */

#define MM_CTXT_ASID_MASK	0x000000ff /* MMU PID reg :8 bit PID */
#define MM_CTXT_CYCLE_MASK	(~MM_CTXT_ASID_MASK)

#define MM_CTXT_FIRST_CYCLE	(MM_CTXT_ASID_MASK + 1)
#define MM_CTXT_NO_ASID		0UL

#define asid_mm(mm, cpu)	mm->context.asid[cpu]
#define hw_pid(mm, cpu)		(asid_mm(mm, cpu) & MM_CTXT_ASID_MASK)

DECLARE_PER_CPU(unsigned int, asid_cache);
#define asid_cpu(cpu)		per_cpu(asid_cache, cpu)


/*
 * Called at the time of execve() to get a new ASID
 * Note the subtlety here: get_new_mmu_context() behaves differently here
 * vs. in switch_mm(). Here it always returns a new ASID, because mm has
 * an unallocated "initial" value, while in latter, it moves to a new ASID,
 * only if it was unallocated
 */
#define activate_mm(prev, next)		switch_mm(prev, next, NULL)

/* it seemed that deactivate_mm( ) is a reasonable place to do book-keeping
 * for retiring-mm. However destroy_context( ) still needs to do that because
 * between mm_release( ) = >deactive_mm( ) and
 * mmput => .. => __mmdrop( ) => destroy_context( )
 * there is a good chance that task gets sched-out/in, making it's ASID valid
 * again (this teased me for a whole day).
 */
#define deactivate_mm(tsk, mm)   do { } while (0)

#define enter_lazy_tlb(mm, tsk)

#endif /* __ASM_ARC_MMU_CONTEXT_H */
