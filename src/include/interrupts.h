#pragma once
#include <stdio.h>
#include <stdbool.h>

struct interrupt_frame;

// those are the isr [interrupt service routine]
// and to be more specific, those are the functions that handle the interrupts
void __attribute__ ((interrupt)) page_fault_handler(struct interrupt_frame* frame);
void __attribute__ ((interrupt)) zero_devision_handler(struct interrupt_frame* frame);
