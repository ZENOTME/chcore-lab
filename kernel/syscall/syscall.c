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

#include <common/types.h>
#include <common/uart.h>
#include <common/uaccess.h>
#include <common/kmalloc.h>
#include <common/mm.h>
#include <common/kprint.h>
#include "syscall_num.h"

void sys_debug(long arg)
{
	kinfo("[syscall] sys_debug: %lx\n", arg);
}

void sys_putc(char ch)
{
	/*
	 * Lab3: Your code here
	 * Send ch to the screen in anyway as your like
	 */
	uart_send(ch);
}

/*
 * Lab3: Your code here
 * Update the syscall table as you like to redirect syscalls
 * to functions accordingly
 */
const void *syscall_table[NR_SYSCALL] = {
	/*
	#define SYS_putc				0
	#define SYS_getc				1
	#define SYS_yield				2
	#define SYS_exit				3
	#define SYS_sleep				4
	#define SYS_create_pmo				5
	#define SYS_map_pmo				6
	#define SYS_create_thread			7
	#define SYS_create_process			8
	#define SYS_register_server			9
	#define SYS_register_client			10
	#define SYS_get_conn_stack			11
	#define SYS_ipc_call				12
	#define SYS_ipc_return				13
	#define SYS_cap_copy_to				15
	#define SYS_cap_copy_from			16
	#define SYS_unmap_pmo				17
	#define SYS_set_affinity                        18
	#define SYS_get_affinity                        19
	#define SYS_create_device_pmo			20
	*/
	[SYS_putc] = sys_putc,
	[SYS_exit] = sys_exit,
	[SYS_map_pmo] = sys_map_pmo,
	[SYS_create_pmo]=sys_create_pmo,
	[SYS_map_pmo]=sys_map_pmo,
	[SYS_handle_brk]=sys_handle_brk,

	
	
	/* lab3 syscalls finished */
};
