#include "include/interrupts.h"

void __attribute__ ((interrupt)) page_fault_handler(struct interrupt_frame* frame)
{
    printf("Page Fault detected :[\n");

    while(true);
}