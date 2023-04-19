#include "gdt.h"
#include <stdint.h>
#include <string.h>

extern void cpu_load_gdt(uint64_t* gdt);
extern void cpu_load_tr();

tss_entry_t tss_entry;
gdt_desc_t gdt_desc;

gdt_entry_t gdt[] = 
{
    // null
    {.limit_low = 0, .base_low = 0, .base_mid = 0, .access = 0, .granularity = 0, .base_high = 0},
    // kernel code
    {.limit_low = 0, .base_low = 0, .base_mid = 0, .access = 0, .granularity = 0, .base_high = 0},
    // kernel data
    {.limit_low = 0, .base_low = 0, .base_mid = 0, .access = 0, .granularity = 0, .base_high = 0},
    // user code
    {.limit_low = 0, .base_low = 0, .base_mid = 0, .access = 0, .granularity = 0, .base_high = 0},
    // user data
    {.limit_low = 0, .base_low = 0, .base_mid = 0, .access = 0, .granularity = 0, .base_high = 0},
    // tss
    {.limit_low = 0, .base_low = 0, .base_mid = 0, .access = 0, .granularity = 0, .base_high = 0},
};

void set_gdt_gate(uint8_t num, uint32_t base, uint16_t limit, uint8_t access, uint8_t gran)
{
    // Setup the descriptor base address
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_mid = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    // Setup the descriptor limits 
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    // Finally, set up the granularity and access flags
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

static void write_tss(uint8_t num, uint16_t ss0, uint64_t esp0)
{
    uint32_t base = (uint32_t) &tss_entry;
    uint16_t limit = base + sizeof(tss_entry);

    set_gdt_gate(num, base, limit, 0xE9, 0x00);
    memset((void *) &tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;
    tss_entry.cs = 0x0B;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

// void init_gdt()
// {
//     gdt_desc.limit = (sizeof(gdt_desc_t) * 6) - 1;
//     gdt_desc.base = (uint64_t) &gdt;

//     set_gdt_gate(0, 0, 0, 0, 0);
//     set_gdt_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
//     set_gdt_gate(1, 0, 0xFFFFFFFF, 0x92, 0xCF);
//     set_gdt_gate(1, 0, 0xFFFFFFFF, 0xFA, 0xCF);
//     set_gdt_gate(1, 0, 0xFFFFFFFF, 0xF2, 0xCF);
//     write_tss(5, 0x10, 0x0);

//     gdt_flush(&gdt_desc);
//     cpu_load_tr();
// }

void help_me()
{
    //
}