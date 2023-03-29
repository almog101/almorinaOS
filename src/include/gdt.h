#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

typedef union 
{
	struct 
  {
	uint16_t limit_low;      // Lower 16 bits of the limit
	uint16_t base_low;       // Lower 16 bits of the base
	uint8_t base_mid;        // Middle 8 bits of the base
	uint8_t type : 4;        // Segment type
	uint8_t desc_type : 1;   // Descriptor type (0 = system, 1 = code/data)
	uint8_t cpl : 2;         // Descriptor privilege level
	uint8_t present : 1;     // Segment present
	uint8_t limit_high : 4;  // Last 4 bits of the limit
	uint8_t available : 1;   // Available for system use
	uint8_t long_desc : 1;   // 64-bit code segment (IA-32e mode only)
	uint8_t op_size : 1;     // 0 = 16-bit | 1 = 32-bit
	uint8_t granularity : 1; // 0 = 1 B | 1 = 4 KiB
	uint8_t base_high;       // Last 8 bits of the base
	};
	uint64_t raw;
} __attribute__ ((__packed__)) gdt_entry_t;

// A struct describing a Task State Segment.
typedef struct tss_entry_struct
{
  uint32_t prev_tss;  // The previous TSS - if we used hardware task switching this would form a linked list.
  uint32_t esp0;      // The stack pointer to load when we change to kernel mode.
  uint32_t ss0;       // The stack segment to load when we change to kernel mode.
  uint32_t esp1;
  uint32_t ss1;
  uint32_t esp2;
  uint32_t ss2;
  uint32_t cr3;
  uint32_t eip;
  uint32_t eflags;
  uint32_t eax;
  uint32_t ecx;
  uint32_t edx;
  uint32_t ebx;
  uint32_t esp;
  uint32_t ebp;
  uint32_t esi;
  uint32_t edi;
  uint32_t es;
  uint32_t cs;
  uint32_t ss;
  uint32_t ds;
  uint32_t fs;
  uint32_t gs;
  uint32_t ldt; 
  uint16_t trap;
  uint16_t iomap_base;
} __attribute__((packed)) tss_entry_t;

typedef struct 
{
  uint16_t limit;
  uint64_t base;
} __attribute__ ((__packed__)) gdt_desc_t;

static void init_gdt();

#endif
