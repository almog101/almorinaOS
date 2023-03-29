#include <gdt.h>
#include <stdint.h>

extern void tss_flush();

static void write_tss();

tss_entry_t tss_entry;

static void init_gdt()
{
  
}