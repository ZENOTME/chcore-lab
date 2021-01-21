/*
 * Copyright (c) 2020 Institute of Parallel And Distributed Systems (IPADS), Shanghai Jiao Tong University (SJTU)
 * OS-Lab-2020 (i.e., ChCore) is licensed under the Mulan PSL v1.
 * You can use this software according to the terms and conditions of the Mulan PSL v1.
 * You may obtain a copy of Mulan PSL v1 at:
 *   http://license.coscl.org.cn/MulanPSL
 *   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 *   PURPOSE.
 *   See the Mulan PSL v1 for more details.
 */

#include <common/mm.h>
#include <common/kprint.h>
#include <common/macro.h>

#include "buddy.h"
#include "slab.h"
#include "page_table.h"

#include <common/kmalloc.h>
extern unsigned long *img_end;

#define PHYSICAL_MEM_START (24*1024*1024)	//24M

#define START_VADDR phys_to_virt(PHYSICAL_MEM_START)	//24M
#define NPAGES (128*1000)

#define PHYSICAL_MEM_END (PHYSICAL_MEM_START+NPAGES*BUDDY_PAGE_SIZE)

/*
 * Layout:
 *
 * | metadata (npages * sizeof(struct page)) | start_vaddr ... (npages * PAGE_SIZE) |
 *
 */

unsigned long get_ttbr1(void)
{
	unsigned long pgd;

	__asm__("mrs %0,ttbr1_el1":"=r"(pgd));
	return pgd;
}

/*
 * map_kernel_space: map the kernel virtual address
 * [va:va+size] to physical addres [pa:pa+size].
 * 1. get the kernel pgd address
 * 2. fill the block entry with corresponding attribution bit
 * kernel_spcae Only 1G
 */
static void map_single_kernel_block(ptp_t* ttbr1_ptp,vaddr_t va, paddr_t pa){
	pte_t* entry;
	/*from L2 get L2 entry(block)*/
	entry=&ttbr1_ptp->ent[GET_L2_INDEX(va)];
	entry->pte=0;
	/*set addr and attr*/
	entry->l2_block.is_valid=1;
	entry->l2_block.attr_index=4;
	entry->l2_block.AF=1;
	entry->l2_block.UXN=1;
	entry->l2_block.pfn=pa>>21;
}

void map_kernel_space(vaddr_t va, paddr_t pa, size_t len)
{
	BUG_ON(va+len-KBASE>0x40000000);
	// <lab2>
	/*get level2*/
	pte_t* entry;
	paddr_t ttbr1=get_ttbr1();
	ptp_t *ttbr1_ptp=(ptp_t *)phys_to_virt(ttbr1);
	entry=&(ttbr1_ptp->ent[GET_L0_INDEX(va)]);
	ttbr1_ptp=(ptp_t *)GET_NEXT_PTP(entry);
	entry=&(ttbr1_ptp->ent[GET_L1_INDEX(va)]);
	ttbr1_ptp=(ptp_t *)GET_NEXT_PTP(entry);
	//kinfo("TEST %lx\n",&entry);
	paddr_t pa_begin=ROUND_DOWN(pa,L2_BLOCK_SIZE);
	paddr_t pa_end=ROUND_UP(pa+len,L2_BLOCK_SIZE);
	paddr_t va_begin=ROUND_DOWN(va,L2_BLOCK_SIZE);
	paddr_t va_end=ROUND_UP(va+len,L2_BLOCK_SIZE);
	int nblocks=(va_end-va_begin)/L2_BLOCK_SIZE;
	BUG_ON(nblocks!=(pa_end-pa_begin)/L2_BLOCK_SIZE);
	for(int i=0;i<nblocks;i++){
		map_single_kernel_block(ttbr1_ptp,va_begin+i*L2_BLOCK_SIZE, pa_begin+i*L2_BLOCK_SIZE);
	}
	// </lab2>
	flush_tlb();
}

void kernel_space_check(void)
{
	unsigned long kernel_val;
	for (unsigned long i = 128; i < 256; i++) {
		kernel_val = *(unsigned long *)(KBASE + (i << 21));
	}
	kinfo("kernel space check pass\n");
}

struct phys_mem_pool global_mem;

void mm_init(void)
{
	vaddr_t free_mem_start = 0;
	struct page *page_meta_start = NULL;
	u64 npages = 0;
	u64 start_vaddr = 0;

	free_mem_start =
	    phys_to_virt(ROUND_UP((vaddr_t) (&img_end), PAGE_SIZE));
	npages = NPAGES;
	start_vaddr = START_VADDR;
	kdebug("[CHCORE] mm: free_mem_start is 0x%lx,data_start is 0x%lx, free_mem_end is 0x%lx\n",
	       free_mem_start, start_vaddr,phys_to_virt(PHYSICAL_MEM_END));

	/*memory layout
	 *---512M------free_mem_end(PHYSICAL_MEM_END)
	 * data 
 	 *---256M---- 
	 * data
	 *---24M------(START_VADDR)
	 * matedata
	 *---1M------free_mem_start 
	 *-----------img_end
	 */
	if ((free_mem_start + npages * sizeof(struct page)) > start_vaddr) {
		BUG("kernel panic: init_mm metadata is too large!\n");
	}

	page_meta_start = (struct page *)free_mem_start;
	kdebug("page_meta_start: 0x%lx, real_start_vadd: 0x%lx,"
	       "npages: 0x%lx, meta_page_size: 0x%lx\n",
	       page_meta_start, start_vaddr, npages, sizeof(struct page));
	/* buddy alloctor for managing physical memory */
	init_buddy(&global_mem, page_meta_start, start_vaddr, npages);

	/* slab alloctor for allocating small memory regions */
	init_slab();

	//map_kernel_space(KBASE + (128UL << 21), 128UL << 21, 128UL << 21);
	//check whether kernel space [KABSE + 256 : KBASE + 512] is mapped 
	kernel_space_check();

}
