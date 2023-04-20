#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <idt.h>
#include <shell.h>
#include <interrupts.h>
#include <idt.h>
#include <heap.h>
#include "bitset.h"
#include <fs.h>
#include <scheduler.h>
#include "pit.h"

idt_register idt_r;
extern idt_descriptor_entry _idt[256];
fs_superblock_t* device;

void prepare_interrupts();
extern int sse_enable(void);

extern int64_t _syscall(int64_t syscall_num, int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3);

void kernel_main(unsigned long magic, unsigned long addr) 
{
	sse_enable();
	prepare_interrupts();
	initialize_memory(magic, addr);
	scheduler_init();
	initialize_syscalls();
	ramfs_device = fs_initialize(100, 30);
	ramfs_root =  fs_create_inode(ramfs_device, INODE_TYPE_DIR);
	fs_dir_add_entry(ramfs_device, ramfs_root, "root", INODE_TYPE_DIR);

	cls();
	print_greetings();

	char* message = "a content";
	char* ret_val = malloc(strlen("a content")) + 1;

	ret_val = _syscall(1, 0, message, strlen(message), 0);	// read
	_syscall(2, 1, ret_val, strlen(ret_val), 0);			// write							// write
	_syscall(2, ret_val, "message", strlen("message"), 0);	// write
	printf("%s\n", ret_val);

	shell_main();
}

/// Activate interrupts.
void prepare_interrupts()
{
	idt_r.limit = 0x0fff;
	idt_r.offset = (uint64_t)_idt;

	idt_set_gate((void*)page_fault_handler, 0xE, IDT_TA_InterruptGate, 0x08, &idt_r);
	idt_set_gate((void*)zero_devision_handler, 0x0, IDT_TA_InterruptGate, 0x08, &idt_r);
	idt_set_gate((void*)key_board_handler, 0x21, IDT_TA_InterruptGate, 0x08, &idt_r);
    idt_set_gate((void*)PIT_tick_handler, 0x20, IDT_TA_InterruptGate, 0x08, &idt_r);
    idt_set_gate((void*)syscall_handler, 0x80, IDT_TA_InterruptGate, 0x08, &idt_r);

	asm("lidt %0" : : "m" (idt_r));

	remap_pic();
	out_b(PIC1_DATA, 0b11111100);
	out_b(PIC2_DATA, 0b11111111);

	asm ("sti");
}
