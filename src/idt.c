#include "include/idt.h"

/*
saves the first 16 bits of offset
then the rest 16 bits
and then the last 32 bits
from the given offset to the given descriptor
*/
void set_offset(idt_descriptor_entry* descriptor, uint64_t offset)
{
    descriptor->offset_low = (uint16_t)(offset & 0x000000000000ffff);
    descriptor->offset_mid = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
    descriptor->offset_high = (uint32_t)((offset & 0xffffffff00000000) >> 32);
}

/*
copies to a variable all the 32 bits of the offset of the given descriptor
and than returns the variable that contains the given descriptor's offset
*/
uint64_t get_offset(idt_descriptor_entry* descriptor)
{
    uint64_t offset = 0;

    offset |= (uint64_t)(descriptor->offset_low);
    offset |= (uint16_t)(descriptor->offset_mid) << 16;
    offset |= (uint16_t)(descriptor->offset_high) << 32;

    return offset;
}
