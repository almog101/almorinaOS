#include "include/interrupts.h"

void __attribute__ ((interrupt)) page_fault_handler(struct interrupt_frame* frame)
{
    printf("Page Fault detected :[\n");

    while(true);
}

void __attribute__ ((interrupt)) zero_devision_handler(struct interrupt_frame* frame)
{
    printf("You cannot devide by zero, silly :P\n\n");
    
    while(true);
}