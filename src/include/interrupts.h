#pragma once
#include <stdio.h>
#include<stdbool.h>

struct interrupt_frame;
void __attribute__ ((interrupt)) page_fault_handler(struct interrupt_frame* frame);
