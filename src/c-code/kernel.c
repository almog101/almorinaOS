#include <stdio.h>
#include <string.h>
#include <idt.h>
#include <shell.h>
#include <interrupts.h>
#include <heap.h>

const char ascii_data[] = {0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x0A, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x7C, 0x0A, 0x2F, 0x20, 0x2F, 0x5F, 0x5C, 0x20, 0x5C, 0x20, 0x7C, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x5C, 0x20, 0x60, 0x2D, 0x2D, 0x2E, 0x20, 0x0A, 0x7C, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x27, 0x5F, 0x20, 0x60, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x7C, 0x20, 0x27, 0x5F, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x27, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x5F, 0x60, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x60, 0x2D, 0x2D, 0x2E, 0x20, 0x5C, 0x0A, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x7C, 0x20, 0x5C, 0x20, 0x5C, 0x5F, 0x2F, 0x20, 0x2F, 0x5C, 0x5F, 0x5F, 0x2F, 0x20, 0x2F, 0x0A, 0x5C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x2F, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x7C, 0x5F, 0x7C, 0x20, 0x20, 0x7C, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, '\n'};

idt_register idt_r;
extern idt_descriptor_entry _idt[256];

void prepare_interrupts();

void kernel_main(unsigned long magic, unsigned long addr) 
{
	prepare_interrupts();
	initialize_memory(magic, addr);

	cls();
	printf("Welcome to\n");
	for (int i = 0; i < sizeof(ascii_data); i++)
		putc(ascii_data[i]);


	char* str = (char*)malloc(10);
	memset(str, 0, 10);
	strcpy(str, "test str");

	printf("new string: %s\n", str);
	free(str);
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

	asm("lidt %0" : : "m" (idt_r));
}
