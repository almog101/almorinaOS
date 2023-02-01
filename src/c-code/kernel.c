#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <idt.h>
#include <shell.h>
#include <interrupts.h>
#include <heap.h>
#include "bitset.h"
#include "fs.h"

idt_register idt_r;
extern idt_descriptor_entry _idt[256];
fs_superblock_t* device;

void prepare_interrupts();

extern int sse_enable(void);

void kernel_main(unsigned long magic, unsigned long addr) 
{
	prepare_interrupts();
	int sse = sse_enable();
	initialize_memory(magic, addr);
	ramfs_device = fs_initialize(100, 30);
	//ramfs_root = fs_dir_add_entry(ramfs_device, 0, "/dir", INODE_TYPE_DIR);
	ramfs_root =  fs_create_inode(ramfs_device, INODE_TYPE_DIR);
	fs_dir_add_entry(ramfs_device, ramfs_root, "root", INODE_TYPE_DIR);

	cls();
	float a = 5123/2.3;
	printf("here is a float %f\n", 3.123f);

	print_greetings();
	shell_main();
}

/// Activate interrupts.
void prepare_interrupts()
{
	idt_r.limit = 0x0fff;
	idt_r.offset = (uint64_t)_idt;

	idt_descriptor_entry* interrupt_page_fault = (idt_descriptor_entry*)(idt_r.offset + 0xE * sizeof(idt_descriptor_entry));
	set_offset(interrupt_page_fault, (uint64_t)page_fault_handler);
	interrupt_page_fault->type_attr = IDT_TA_InterruptGate;
	interrupt_page_fault->selector = 0x08;

	idt_descriptor_entry* interrupt_zero_devision = (idt_descriptor_entry*)(idt_r.offset + 0x0 * sizeof(idt_descriptor_entry));
	set_offset(interrupt_zero_devision, (uint64_t)zero_devision_handler);
	interrupt_zero_devision->type_attr = IDT_TA_InterruptGate;
	interrupt_zero_devision->selector = 0x08;

	idt_descriptor_entry* interrupt_keyboard = (idt_descriptor_entry*)(idt_r.offset + 0x21 * sizeof(idt_descriptor_entry));
	set_offset(interrupt_keyboard, (uint64_t)key_board_handler);
	interrupt_keyboard->type_attr = IDT_TA_InterruptGate;
	interrupt_keyboard->selector = 0x08;

	asm("lidt %0" : : "m" (idt_r));

	remap_pic();
	out_b(PIC1_DATA, 0b11111101);
	out_b(PIC2_DATA, 0b11111111);

	asm ("sti");
}
