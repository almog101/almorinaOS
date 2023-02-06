#pragma once
#include <stdio.h>

// types of idt gates [self explanatory]
#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111

// data structure to represent the interrupt descriptor
// needed for handling interrupts
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

// set & get offset of the interrupt descriptor
// needed for convenience purposes only
void set_offset(idt_descriptor_entry* descriptor, uint64_t offset);
uint64_t get_offset(idt_descriptor_entry* descriptor);

// data structure to represent an idt register
// needed for calling the descriptor
struct __attribute__ ((__packed__)) idt_register
{
    uint16_t limit;
    uint64_t offset;
} typedef idt_register;

void idt_set_gate(void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector, idt_register* idtr);
