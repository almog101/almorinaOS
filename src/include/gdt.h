#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

typedef union {
	struct {
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

struct __attribute__ ((__packed__)) tss {
  uint32_t : 32;      // reserved
  uint64_t rsp0;      // rsp0
  uint64_t rsp1;      // rsp1
  uint64_t rsp2;      // rsp2
  uint64_t : 64;      // reserved
  uint64_t ist1;      // ist1
  uint64_t ist2;      // ist2
  uint64_t ist3;      // ist3
  uint64_t ist4;      // ist4
  uint64_t ist5;      // ist5
  uint64_t ist6;      // ist6
  uint64_t ist7;      // ist7
  uint64_t : 32;      // reserved
  uint16_t : 16;      // reserved
  uint16_t iopb_ofst; // iopb offset
};

typedef struct {
  uint16_t limit;
  uint64_t base;
} __attribute__ ((__packed__)) gdt_desc_t;


void setup_gdt();

#endif
