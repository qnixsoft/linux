// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012 Regents of the University of California
 * Copyright (C) 2019 Western Digital Corporation or its affiliates.
 */

#include <linux/init.h>
#include <linux/mm.h>
#include <linux/memblock.h>
#include <linux/initrd.h>
#include <linux/swap.h>
#include <linux/sizes.h>
#include <linux/of_fdt.h>
#include <linux/libfdt.h>
#include <linux/set_memory.h>

#include <asm/fixmap.h>
#include <asm/tlbflush.h>
#include <asm/sections.h>
#include <asm/soc.h>
#include <asm/io.h>
#include <asm/ptdump.h>

#include "../kernel/head.h"

unsigned long empty_zero_page[PAGE_SIZE / sizeof(unsigned long)]
							__page_aligned_bss;
EXPORT_SYMBOL(empty_zero_page);

void *dtb_early_va;

static void __init zone_sizes_init(void)
{
	unsigned long max_zone_pfns[MAX_NR_ZONES] = { 0, };

	max_zone_pfns[ZONE_NORMAL] = max_low_pfn;

	free_area_init(max_zone_pfns);
}

static void setup_zero_page(void)
{
	memset((void *)empty_zero_page, 0, PAGE_SIZE);
}

static void print_vm_layout(void) { }

void __init mem_init(void)
{
#ifdef CONFIG_FLATMEM
	BUG_ON(!mem_map);
#endif /* CONFIG_FLATMEM */

	high_memory = (void *)(__va(PFN_PHYS(max_low_pfn)));
	memblock_free_all();

	mem_init_print_info(NULL);
	print_vm_layout();
}

#ifdef CONFIG_BLK_DEV_INITRD
static void __init setup_initrd(void)
{
	unsigned long size;

	if (initrd_start >= initrd_end) {
		pr_info("initrd not found or empty");
		goto disable;
	}
	if (__pa_symbol(initrd_end) > PFN_PHYS(max_low_pfn)) {
		pr_err("initrd extends beyond end of memory");
		goto disable;
	}

	size = initrd_end - initrd_start;
	memblock_reserve(__pa_symbol(initrd_start), size);
	initrd_below_start_ok = 1;

	pr_info("Initial ramdisk at: 0x%p (%lu bytes)\n",
		(void *)(initrd_start), size);
	return;
disable:
	pr_cont(" - disabling initrd\n");
	initrd_start = 0;
	initrd_end = 0;
}
#endif /* CONFIG_BLK_DEV_INITRD */

static phys_addr_t dtb_early_pa __initdata;

extern phys_addr_t memory_len;

void __init setup_bootmem(void)
{
	struct memblock_region *reg;
	/* phys_addr_t mem_size = 0; */
	phys_addr_t mem_size = (unsigned) -1;

	pr_devel("setup_bootmem start=0, end=%u\n", mem_size);

	if (memblock_add(1 << 16, 1 << 20))
		pr_info("memblock_add failed\n");

	set_max_mapnr(PFN_DOWN(mem_size));
	max_pfn = PFN_DOWN(memblock_end_of_DRAM());
	max_low_pfn = max_pfn;

#ifdef CONFIG_BLK_DEV_INITRD
	setup_initrd();
#endif /* CONFIG_BLK_DEV_INITRD */

	memblock_allow_resize();
	memblock_dump_all();
}

asmlinkage void __init setup_vm(uintptr_t dtb_pa)
{
#ifdef CONFIG_BUILTIN_DTB
	dtb_early_va = soc_lookup_builtin_dtb();
	if (!dtb_early_va) {
		/* Fallback to first available DTS */
		dtb_early_va = (void *) __dtb_start;
	}
#else
	dtb_early_va = (void *)dtb_pa;
#endif
}

static inline void setup_vm_final(void)
{
}

void __init paging_init(void)
{
	setup_vm_final();
	memblocks_present();
	sparse_init();
	setup_zero_page();
	zone_sizes_init();
}

#ifdef CONFIG_SPARSEMEM_VMEMMAP
int __meminit vmemmap_populate(unsigned long start, unsigned long end, int node,
			       struct vmem_altmap *altmap)
{
	return vmemmap_populate_basepages(start, end, node);
}
#endif

bool __init early_init_dt_scan(void *params)
{
#if 0
	if (!params)
		return false;

	/* Setup flat device-tree pointer */
	initial_boot_params = params;

	/* check device tree validity */
	if (be32_to_cpu(initial_boot_params->magic) != OF_DT_HEADER) {
		initial_boot_params = NULL;
		return false;
	}

	/* Retrieve various information from the /chosen node */
	of_scan_flat_dt(early_init_dt_scan_chosen, boot_command_line);

	/* Initialize {size,address}-cells info */
	of_scan_flat_dt(early_init_dt_scan_root, NULL);

	/* Setup memory, calling early_init_dt_add_memory_arch */
	of_scan_flat_dt(early_init_dt_scan_memory, NULL);
#endif

	return true;
}
