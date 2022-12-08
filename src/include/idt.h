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
} typedef idt64;

void initialize_idt64();
void isr1_handler();
