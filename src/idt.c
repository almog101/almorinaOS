#include "idt.h"

extern idt64 _idt[256];
extern uint64_t isr1;
extern void load_idt();

extern void initialize_idt64()
{
    for(uint64_t t = 0; t < 256; t++)
    {
        _idt[t].zero = 0;
        _idt[t].offset_low = (uint16_t)(((uint64_t)&isr1 & 0x000000000000FFFF));
        _idt[t].offset_mid = (uint16_t)(((uint64_t)&isr1 & 0x00000000FFFF0000) >> 16);
        _idt[t].offset_high = (uint16_t)(((uint64_t)&isr1 & 0xFFFFFFFF00000000) >> 32);
        _idt[t].ist = 0;
        _idt[t].selector = 0x08;
        _idt[t].type_attr = IDT_TA_InterruptGate;
    }

    outb(0x21, 0xfd);
    outb(0xa1, 0xff);
    load_idt();
}

extern void isr1_handler()
{
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}
