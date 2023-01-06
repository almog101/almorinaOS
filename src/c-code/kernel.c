#include <stdio.h>
#include <string.h>
#include <multiboot2.h>
#include <mmap.h>
#include <heap.h>
#include <shell.h>
#include <interrupts.h>
#include <idt.h>

const char ascii_data[] = {0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x0A, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x7C, 0x0A, 0x2F, 0x20, 0x2F, 0x5F, 0x5C, 0x20, 0x5C, 0x20, 0x7C, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x5C, 0x20, 0x60, 0x2D, 0x2D, 0x2E, 0x20, 0x0A, 0x7C, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x27, 0x5F, 0x20, 0x60, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x7C, 0x20, 0x27, 0x5F, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x27, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x5F, 0x60, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x60, 0x2D, 0x2D, 0x2E, 0x20, 0x5C, 0x0A, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x7C, 0x20, 0x5C, 0x20, 0x5C, 0x5F, 0x2F, 0x20, 0x2F, 0x5C, 0x5F, 0x5F, 0x2F, 0x20, 0x2F, 0x0A, 0x5C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x2F, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x7C, 0x5F, 0x7C, 0x20, 0x20, 0x7C, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, '\n', 0};
idt_register idt_r;
extern idt_descriptor_entry _idt[256];

void prepare_interrupts();

void kernel_main(unsigned long magic, unsigned long addr) 
{
	prepare_interrupts();
	cls();
	printf("Welcome to\n%s", ascii_data);

	/* Make sure the magic number matches for memory mapping*/
	if(magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
		puts("invalid magic number!");
		return;
	}

	extract_mmap(addr);
	multiboot_memory_map_t* ent = get_mmap_entery(0);
	initialize_heap(ent->addr, ent->len);
	printf("\n[Heap initialized. address: 0x%x, length: 0x%x]\n\n\מ", ent->addr, ent->len);

	shell_main();
}

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
