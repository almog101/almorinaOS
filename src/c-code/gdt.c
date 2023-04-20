#include <gdt.h>
#include <stdint.h>

#define segment(base, limit, typ, s, dpl, p, is64, is32, g)  \
  ((gdt_entry_t) {                                            \
    .limit_low = (limit) & 0xFFFF, .base_low = (base) & 0xFFFF, \
    .base_mid = (base) >> 16, .type = typ, .desc_type = s, .cpl = dpl, \
    .present = p, .limit_high = (limit) >> 16, .available = 0, .long_desc = is64, \
    .op_size = is32, .granularity = g, .base_high = (base) >> 24 \
  })

#define system_segment_low(base, limit, typ, s, dpl, p, is64, is32, g) \
  ((gdt_entry_t) {                                                 \
    .limit_low = (limit) & 0xFFFF, .base_low = (base) & 0xFFFF, \
    .base_mid = (base) >> 16, .type = typ, .desc_type = s, .cpl = dpl, \
    .present = p, .limit_high = (limit) >> 16, .available = 1, .long_desc = is64, \
    .op_size = is32, .granularity = g, .base_high = (base) >> 24 \
  })

#define system_segment_high(base) \
  ((gdt_entry_t) { \
    .raw = (base) >> 32 \
  })

#define segment_type(b0, b1, b2, b3) \
  (((b0) << 0) | ((b1) << 1) | ((b2) << 2) | ((b3) << 3))

// segment types

#define null_segment() \
  segment(0, 0, 0, 0, 0, 0, 0, 0, 0)

#define code_segment(base, limit, dpl, read, c, is64, is32, g) \
  segment(base, limit, segment_type(0, read, c, 1), 1, dpl, 1, is64, is32, g)

#define data_segment(base, limit, dpl, write, e, is64, is32, g) \
  segment(base, limit, segment_type(0, write, e, 0), 1, dpl, 1, is64, is32, g)

// 64-bit segments

#define code_segment64(ring) \
  code_segment(0, 0, ring, 1, 0, 1, 0, 1)

#define data_segment64(ring) \
  data_segment(0, 0, ring, 1, 0, 0, 1, 1)

#define tss_segment_low(base) \
  system_segment_low(base, 0, segment_type(1, 0, 0, 1), 0, 0, 1, 0, 0, 1)

#define tss_segment_high(base) \
  system_segment_high(base)

gdt_entry_t gdt[] = {
  null_segment(),    // 0x00 null
  code_segment64(0), // 0x08 kernel code
  data_segment64(0), // 0x10 kernel data
  data_segment64(3), // 0x18 user data
  code_segment64(3), // 0x20 user code
  null_segment(),    // 0x28 tss low
  null_segment(),    // 0x30 tss high
};

uint8_t ring0_stack[4096];
struct tss tss;

void setup_gdt() {
	gdt_desc_t gdt_desc;
	memset((void *) &tss, 0, sizeof(tss));
	tss.rsp0 = (uint64_t) ring0_stack;

	gdt[5] = tss_segment_low((uint64_t) &tss);
	gdt[6] = tss_segment_high((uint64_t) &tss);

	gdt_desc.limit = sizeof(gdt) - 1;
	gdt_desc.base = (uint64_t) &gdt;

	cpu_load_gdt(&gdt_desc);
	cpu_load_tr(0x28);
	cpu_reload_segments();
}
