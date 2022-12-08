#pragma once
#include <stdio.h>

// self explanatory
#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111

struct idt64
{
    uint16_t offset_low;            // lower bits
    uint16_t selector;              // select the segment
    uint8_t ist;                    // interurpt stack table
    uint8_t type_attr;              // points at gate type (interrupt, trap or call)
    uint16_t offset_mid;            // next 16 bits of offset0
    uint32_t offset_high;           // upper bits
    uint32_t zero;                  // do not use
};

extern struct idt64 _idt[256];
extern uint64_t isr1;
extern "C" void load_idt();

void initialize_idt64()
{
    for(unit64_t t = 0; t < 256; i++)
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
    initialize_idt64();
}

extern "C" void isr1_handler()
{
    printf("keybaord interrupt??");
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}