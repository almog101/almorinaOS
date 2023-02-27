#include "../include/interrupts.h"
#include "../include/stdio.h"
#include "../include/stdint.h"
#include "../include/io.h"
#include "../include/keyboard.h"
#include "../include/vga.h"
#include "pit.h"
#include "scheduler.h"

/*
for now, prints a warning message about the page fault
and denies the user from doing any further actions
for their own safaty *safisfied emoji
*/
void __attribute__ ((interrupt)) page_fault_handler(struct interrupt_frame* frame)
{
    set_fg_color(WHITE);
    printf("ERROR: Page Fault detected :[\n");

    while(true);
}

/*
does the same as the page fault handler
but the message warns anout the devision by zero
[oh my, what a crime]
*/
void __attribute__ ((interrupt)) zero_devision_handler(struct interrupt_frame* frame)
{
    set_fg_color(WHITE);
    printf("ERROR: You cannot devide by zero, silly :P\n\n");
    
    while(true);
}

void __attribute__ ((interrupt)) key_board_handler(struct interrupt_frame* frame)
{
    uint8_t scancode = in_b(0x60);
    keyboard_handler(scancode);

    pic_end_master();
}


void __attribute__ ((interrupt)) PIT_tick_handler(struct interrupt_frame* frame)
{
    PIT_tick();
    pic_end_master();
	irq_schedule_handler();
}

void remap_pic()
{
    uint8_t a1, a2;

    a1 = in_b(PIC1_DATA);
    io_wait();
    a2 = in_b(PIC2_DATA);
    io_wait();

    out_b(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    out_b(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    out_b(PIC1_DATA, 0x20);
    io_wait();
    out_b(PIC2_DATA, 0x28);
    io_wait();

    out_b(PIC1_DATA, 4);
    io_wait();
    out_b(PIC2_DATA, 2);
    io_wait();

    out_b(PIC1_DATA, ICW4_8086);
    io_wait();
    out_b(PIC2_DATA, ICW4_8086);
    io_wait();

    out_b(PIC1_DATA, a1);
    io_wait();
    out_b(PIC2_DATA, a2);
    io_wait();
}

void pic_end_master()
{
    out_b(PIC1_COMMAND, PIC_EOI);
}

void pic_end_slave()
{
    out_b(PIC2_COMMAND, PIC_EOI);
    out_b(PIC1_COMMAND, PIC_EOI);
}
