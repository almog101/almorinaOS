#include "include/idt.h"

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
