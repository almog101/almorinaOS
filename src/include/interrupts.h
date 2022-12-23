#pragma once
#include <stdio.h>
#include <stdbool.h>

struct interrupt_frame;

// those are the isr [interrupt service routine]
void __attribute__ ((interrupt)) page_fault_handler(struct interrupt_frame* frame);
void __attribute__ ((interrupt)) zero_devision_handler(struct interrupt_frame* frame);
