#include "include/idt.h"

//extern idt64 _idt[256];
//extern uint64_t isr1;
//extern void load_idt();

/*extern void initialize_idt64()
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
}*/

void set_offset(idt_descriptor_entry* descriptor, uint64_t offset)
{
    descriptor->offset_low = (uint16_t)(offset & 0x000000000000ffff);
    descriptor->offset_mid = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
    descriptor->offset_high = (uint32_t)((offset & 0xffffffff00000000) >> 32);
}

uint64_t get_offset(idt_descriptor_entry* descriptor)
{
    uint64_t offset = 0;

    offset |= (uint64_t)(descriptor->offset_low);
    offset |= (uint16_t)(descriptor->offset_mid) << 16;
    offset |= (uint16_t)(descriptor->offset_high) << 32;

    return offset;
}

/*extern void isr1_handler()
{
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}*/