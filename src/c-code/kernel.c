#include <stdio.h>
#include <string.h>
#include <idt.h>
#include <shell.h>
#include <interrupts.h>

const char ascii_data[] = {0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x5F, 0x5F, 0x20, 0x0A, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x7C, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x2F, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x7C, 0x0A, 0x2F, 0x20, 0x2F, 0x5F, 0x5C, 0x20, 0x5C, 0x20, 0x7C, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x5F, 0x20, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x20, 0x5F, 0x5F, 0x20, 0x20, 0x20, 0x5F, 0x5F, 0x20, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x5C, 0x20, 0x60, 0x2D, 0x2D, 0x2E, 0x20, 0x0A, 0x7C, 0x20, 0x20, 0x5F, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x27, 0x5F, 0x20, 0x60, 0x20, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x5F, 0x20, 0x5C, 0x7C, 0x20, 0x27, 0x5F, 0x5F, 0x7C, 0x20, 0x7C, 0x20, 0x27, 0x5F, 0x20, 0x5C, 0x20, 0x2F, 0x20, 0x5F, 0x60, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x60, 0x2D, 0x2D, 0x2E, 0x20, 0x5C, 0x0A, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x29, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x7C, 0x20, 0x28, 0x5F, 0x7C, 0x20, 0x5C, 0x20, 0x5C, 0x5F, 0x2F, 0x20, 0x2F, 0x5C, 0x5F, 0x5F, 0x2F, 0x20, 0x2F, 0x0A, 0x5C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x2F, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x7C, 0x5F, 0x7C, 0x20, 0x20, 0x7C, 0x5F, 0x7C, 0x5F, 0x7C, 0x20, 0x7C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x2C, 0x5F, 0x7C, 0x5C, 0x5F, 0x5F, 0x5F, 0x2F, 0x5C, 0x5F, 0x5F, 0x5F, 0x5F, 0x2F, '\n'};

idt_register idt_r;
extern idt_descriptor_entry _idt[256];

void shell_parse(const char* line, int n, char argv[5][20], int argc);
void add(int n1, int n2);
void print(const char* format, char argv[5][20]);
void help();
void prepare_interrupts();

void kernel_main() 
{
	prepare_interrupts();

	//asm("int $0x0e"); // the page fault error
	//int num = 43 / 0;

	cls();
	printf("Welcome to\n");
	for (int i = 0; i < sizeof(ascii_data); i++)
		putc(ascii_data[i]);

	printf("\n\n[command] $ \n");

	/*char data[128] = "add 1 2";
	char args[5][20] = {0};
	
	shell_parse(data, 128, args, 5);*/
}

void shell_parse(const char* line, int n, char argv[5][20], int argc)
{
	uint8_t args_count = 0;
	char* start = line;
	char* end = line;

	do
	{
		end = strchr(start, ' ');
		if (end == 0)
		{
			strcpy(argv[args_count], start);
			break;
		}
		memcpy(argv[args_count], start, (int)(end-start));

		start = end+1;
	} while(args_count++ < argc);
}


void add(int n1, int n2)
{
	printf("%d\n", n1+n2);
}

void print(const char* format, char argv[5][20])
{
	int i = 0;
	int j = 2;
	while (format[i])
	{
		if (format[i] == '%')
		{
			if (format[i+1] == 'd')
				putd(atoi(argv[j++]));
			else if (format[i+1] == 'c')
				putc(argv[j++][0]);
			
			i+=2;
		}
		else
		{
			putc(format[i]);
			i++;
		}
   }
}

void help()
{
	puts("help - this list\n");
	puts("add - takes two numbers and adds. ex. add 1 2\n");
	puts("print - takes a format string a variables. ex. print hello%cworld @\n");
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
