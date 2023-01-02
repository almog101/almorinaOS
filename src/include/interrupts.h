#pragma once
#include <stdbool.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct interrupt_frame;

// those are the isr [interrupt service routine]
// and to be more specific, those are the functions that handle the interrupts
void __attribute__ ((interrupt)) page_fault_handler(struct interrupt_frame* frame);
void __attribute__ ((interrupt)) zero_devision_handler(struct interrupt_frame* frame);
void __attribute__ ((interrupt)) key_board_handler(struct interrupt_frame* frame);

void remap_pic();
void pic_end_master();
void pic_end_slave();
