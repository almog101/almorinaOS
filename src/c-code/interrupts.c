#include "include/interrupts.h"

/*
for now, prints a warning message about the page fault
and denies the user from doing any further actions
for their own safaty *safisfied emoji
*/
void __attribute__ ((interrupt)) page_fault_handler(struct interrupt_frame* frame)
{
    printf("Page Fault detected :[\n");

    while(true);
}

/*
does the same as the page fault handler
but the message warns anout the devision by zero
[oh my, what a crime]
*/
void __attribute__ ((interrupt)) zero_devision_handler(struct interrupt_frame* frame)
{
    printf("You cannot devide by zero, silly :P\n\n");
    
    while(true);
}