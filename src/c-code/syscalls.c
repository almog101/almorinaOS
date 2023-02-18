#include <stdint.h>
#include <stdio.h>
#include "interrupts.h"

void dispatch_syscall(uint16_t num, uint16_t val) {
	printf("sys (0x%x) = 0x%x\n", num, val);
}
