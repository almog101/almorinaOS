#pragma once
#include <stdio.h>

// self explanatory
#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111

struct idt_descriptor_entry
{
    uint16_t offset_low;            // lower bits
    uint16_t selector;              // select the segment
    uint8_t ist;                    // interurpt stack table
    uint8_t type_attr;              // points at gate type (interrupt, trap or call)
    uint16_t offset_mid;            // next 16 bits of offset0
    uint32_t offset_high;           // upper bits
    uint32_t zero;                  // do not use
} typedef idt_descriptor_entry;

void set_offset(idt_descriptor_entry* descriptor, uint64_t offset);
uint64_t get_offset(idt_descriptor_entry* descriptor);

struct __attribute__ ((__packed__)) idt_register
{
    uint16_t limit;
    uint64_t offset;
}typedef idt_register;

//void initialize_idt64();
//void isr1_handler();
