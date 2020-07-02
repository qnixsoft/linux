/*
 * TLB Management (flush/create/diagnostics) for ARC700
 *
 * Copyright (C) 2004, 2007-2010, 2011-2012 Synopsys, Inc. (www.synopsys.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * vineetg: Aug 2011
 *  -Reintroduce duplicate PD fixup - some customer chips still have the issue
 *
 * vineetg: May 2011
 *  -No need to flush_cache_page( ) for each call to update_mmu_cache()
 *   some of the LMBench tests improved amazingly
 *      = page-fault thrice as fast (75 usec to 28 usec)
 *      = mmap twice as fast (9.6 msec to 4.6 msec),
 *      = fork (5.3 msec to 3.7 msec)
 *
 * vineetg: April 2011 :
 *  -MMU v3: PD{0,1} bits layout changed: They don't overlap anymore,
 *      helps avoid a shift when preparing PD0 from PTE
 *
 * vineetg: April 2011 : Preparing for MMU V3
 *  -MMU v2/v3 BCRs decoded differently
 *  -Remove TLB_SIZE hardcoding as it's variable now: 256 or 512
 *  -tlb_entry_erase( ) can be void
 *  -local_flush_tlb_range( ):
 *      = need not "ceil" @end
 *      = walks MMU only if range spans < 32 entries, as opposed to 256
 *
 * Vineetg: Sept 10th 2008
 *  -Changes related to MMU v2 (Rel 4.8)
 *
 * Vineetg: Aug 29th 2008
 *  -In TLB Flush operations (Metal Fix MMU) there is a explict command to
 *    flush Micro-TLBS. If TLB Index Reg is invalid prior to TLBIVUTLB cmd,
 *    it fails. Thus need to load it with ANY valid value before invoking
 *    TLBIVUTLB cmd
 *
 * Vineetg: Aug 21th 2008:
 *  -Reduced the duration of IRQ lockouts in TLB Flush routines
 *  -Multiple copies of TLB erase code seperated into a "single" function
 *  -In TLB Flush routines, interrupt disabling moved UP to retrieve ASID
 *       in interrupt-safe region.
 *
 * Vineetg: April 23rd Bug #93131
 *    Problem: tlb_flush_kernel_range() doesn't do anything if the range to
 *              flush is more than the size of TLB itself.
 *
 * Rahul Trivedi : Codito Technologies 2004
 */

#include <linux/module.h>
#include <linux/bug.h>
#include <linux/mm_types.h>

#include <asm/arcregs.h>
#include <asm/setup.h>
#include <asm/mmu_context.h>
#include <asm/mmu.h>

/*			Need for ARC MMU v2
 *
 * ARC700 MMU-v1 had a Joint-TLB for Code and Data and is 2 way set-assoc.
 * For a memcpy operation with 3 players (src/dst/code) such that all 3 pages
 * map into same set, there would be contention for the 2 ways causing severe
 * Thrashing.
 *
 * Although J-TLB is 2 way set assoc, ARC700 caches J-TLB into uTLBS which has
 * much higher associativity. u-D-TLB is 8 ways, u-I-TLB is 4 ways.
 * Given this, the thrasing problem should never happen because once the 3
 * J-TLB entries are created (even though 3rd will knock out one of the prev
 * two), the u-D-TLB and u-I-TLB will have what is required to accomplish memcpy
 *
 * Yet we still see the Thrashing because a J-TLB Write cause flush of u-TLBs.
 * This is a simple design for keeping them in sync. So what do we do?
 * The solution which James came up was pretty neat. It utilised the assoc
 * of uTLBs by not invalidating always but only when absolutely necessary.
 *
 * - Existing TLB commands work as before
 * - New command (TLBWriteNI) for TLB write without clearing uTLBs
 * - New command (TLBIVUTLB) to invalidate uTLBs.
 *
 * The uTLBs need only be invalidated when pages are being removed from the
 * OS page table. If a 'victim' TLB entry is being overwritten in the main TLB
 * as a result of a miss, the removed entry is still allowed to exist in the
 * uTLBs as it is still valid and present in the OS page table. This allows the
 * full associativity of the uTLBs to hide the limited associativity of the main
 * TLB.
 *
 * During a miss handler, the new "TLBWriteNI" command is used to load
 * entries without clearing the uTLBs.
 *
 * When the OS page table is updated, TLB entries that may be associated with a
 * removed page are removed (flushed) from the TLB using TLBWrite. In this
 * circumstance, the uTLBs must also be cleared. This is done by using the
 * existing TLBWrite command. An explicit IVUTLB is also required for those
 * corner cases when TLBWrite was not executed at all because the corresp
 * J-TLB entry got evicted/replaced.
 */


/* A copy of the ASID from the PID reg is kept in asid_cache */
DEFINE_PER_CPU(unsigned int, asid_cache) = MM_CTXT_FIRST_CYCLE;

static int __read_mostly pae_exists;

/*
 * Utility Routine to erase a J-TLB entry
 * Caller needs to setup Index Reg (manually or via getIndex)
 */
static inline void __tlb_entry_erase(void)
{
        pr_err("__tlb_entry_erase\n");
}

static inline unsigned int tlb_entry_lkup(unsigned long vaddr_n_asid)
{
        pr_err("tlb_entry_lkup %lu\n", vaddr_n_asid);
	return 0;
}

static void tlb_entry_erase(unsigned int vaddr_n_asid)
{
        pr_err("tlb_entry_erase %u\n", vaddr_n_asid);
}

/****************************************************************************
 * ARC700 MMU caches recently used J-TLB entries (RAM) as uTLBs (FLOPs)
 *
 * New IVUTLB cmd in MMU v2 explictly invalidates the uTLB
 *
 * utlb_invalidate ( )
 *  -For v2 MMU calls Flush uTLB Cmd
 *  -For v1 MMU does nothing (except for Metal Fix v1 MMU)
 *      This is because in v1 TLBWrite itself invalidate uTLBs
 ***************************************************************************/

static void utlb_invalidate(void)
{
        pr_err("utlb_invalidate\n");
	panic("utlb_invalidate\n");
}

static void tlb_entry_insert(unsigned int pd0, pte_t pd1)
{

        pr_err("local_flush_tlb_all %u\n", pd0);
	panic("local_flush_tlb_all\n");
}

/*
 * Un-conditionally (without lookup) erase the entire MMU contents
 */

noinline void local_flush_tlb_all(void)
{
        pr_err("local_flush_tlb_all\n");
}

/*
 * Flush the entrie MM for userland. The fastest way is to move to Next ASID
 */
noinline void local_flush_tlb_mm(struct mm_struct *mm)
{
        pr_err("local_flush_tlb_mm %p\n", mm);

	/* destroy_context(mm); */
	/* if (current->mm == mm) */
	/* 	get_new_mmu_context(mm); */
}

/*
 * Flush a Range of TLB entries for userland.
 * @start is inclusive, while @end is exclusive
 * Difference between this and Kernel Range Flush is
 *  -Here the fastest way (if range is too large) is to move to next ASID
 *      without doing any explicit Shootdown
 *  -In case of kernel Flush, entry has to be shot down explictly
 */
void local_flush_tlb_range(struct vm_area_struct *vma, unsigned long start,
			   unsigned long end)
{
        pr_err("local_flush_tlb_range %p %lu %lu\n", vma, start, end);
}

/* Flush the kernel TLB entries - vmalloc/modules (Global from MMU perspective)
 *  @start, @end interpreted as kvaddr
 * Interestingly, shared TLB entries can also be flushed using just
 * @start,@end alone (interpreted as user vaddr), although technically SASID
 * is also needed. However our smart TLbProbe lookup takes care of that.
 */
void local_flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	unsigned long flags;

	/* exactly same as above, except for TLB entry not taking ASID */

	if (unlikely((end - start) >= PAGE_SIZE * 32)) {
		local_flush_tlb_all();
		return;
	}

	start &= PAGE_MASK;

	local_irq_save(flags);
	while (start < end) {
		tlb_entry_erase(start);
		start += PAGE_SIZE;
	}

	utlb_invalidate();

	local_irq_restore(flags);
}

/*
 * Delete TLB entry in MMU for a given page (??? address)
 * NOTE One TLB entry contains translation for single PAGE
 */

void local_flush_tlb_page(struct vm_area_struct *vma, unsigned long page)
{
        pr_err("local_flush_tlb_page %p %lu\n", vma, page);
}

/*
 * Routine to create a TLB entry
 */
void create_tlb(struct vm_area_struct *vma, unsigned long vaddr, pte_t *ptep)
{
        pr_err("create_tlb %p %lu %p\n", vma, vaddr, ptep);
}

/*
 * Called at the end of pagefault, for a userspace mapped page
 *  -pre-install the corresponding TLB entry into MMU
 *  -Finalize the delayed D-cache flush of kernel mapping of page due to
 *  	flush_dcache_page(), copy_user_page()
 *
 * Note that flush (when done) involves both WBACK - so physical page is
 * in sync as well as INV - so any non-congruent aliases don't remain
 */
void update_mmu_cache(struct vm_area_struct *vma, unsigned long vaddr_unaligned,
		      pte_t *ptep)
{
        pr_err("update_mmu_cache %p %lu %p\n", vma, vaddr_unaligned, ptep);
}

/* Read the Cache Build Confuration Registers, Decode them and save into
 * the cpuinfo structure for later use.
 * No Validation is done here, simply read/convert the BCRs
 */
void read_decode_mmu_bcr(void)
{
        pr_err("read_decode_mmu_bcr\n");
}

void arc_mmu_init(void)
{
	pr_err("arc_mmu_init\n");
}

/*
 * TLB Programmer's Model uses Linear Indexes: 0 to {255, 511} for 128 x {2,4}
 * The mapping is Column-first.
 *		---------------------	-----------
 *		|way0|way1|way2|way3|	|way0|way1|
 *		---------------------	-----------
 * [set0]	|  0 |  1 |  2 |  3 |	|  0 |  1 |
 * [set1]	|  4 |  5 |  6 |  7 |	|  2 |  3 |
 *		~		    ~	~	  ~
 * [set127]	| 508| 509| 510| 511|	| 254| 255|
 *		---------------------	-----------
 * For normal operations we don't(must not) care how above works since
 * MMU cmd getIndex(vaddr) abstracts that out.
 * However for walking WAYS of a SET, we need to know this
 */
#define SET_WAY_TO_IDX(mmu, set, way)  ((set) * mmu->ways + (way))

/* Handling of Duplicate PD (TLB entry) in MMU.
 * -Could be due to buggy customer tapeouts or obscure kernel bugs
 * -MMU complaints not at the time of duplicate PD installation, but at the
 *      time of lookup matching multiple ways.
 * -Ideally these should never happen - but if they do - workaround by deleting
 *      the duplicate one.
 * -Knob to be verbose abt it.(TODO: hook them up to debugfs)
 */
volatile int dup_pd_silent; /* Be slient abt it or complain (default) */

void do_tlb_overlap_fault(unsigned long cause, unsigned long address,
			  struct pt_regs *regs)
{
        pr_err("do_tlb_overlap_fault %lu %lu %p\n", cause, address, regs);
}
