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

// Simple command-line kernel monitor useful for
// controlling the kernel and exploring the system interactively.

#include <common/printk.h>
#include <common/types.h>

static inline __attribute__ ((always_inline))
u64 read_fp()
{
	u64 fp;
	__asm __volatile("mov %0, x29":"=r"(fp));
	return fp;
}

static inline __attribute__ ((always_inline))
u64 read_x19()
{
	u64 x19;
	__asm __volatile("mov %0, x19":"=r"(x19));
	return x19;
}


__attribute__ ((optimize("O5")))
int stack_backtrace()
{
	printk("Stack backtrace:\n");
	
	// Your code here.
	
	u64 sp=*(u64 *)(read_fp());
	u64 last_sp=(read_fp());
	u64 lr=*((u64 *)sp+1);
	while(sp){
		printk("LR %lx FP %lx Args %lx %lx %lx %lx %x\n",lr,sp,*((u64 *)last_sp+2),*((u64 *)last_sp+3),*((u64 *)last_sp+4),*((u64 *)last_sp+5),*((u64 *)last_sp+6));
		last_sp=sp;
		sp=*(u64*)sp;
	}

	return 0;
}
