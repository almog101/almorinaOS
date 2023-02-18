#include <stdint.h>
#include <stdio.h>
#include "interrupts.h"

typedef void (*SYSCALL_FUNCTION) (uint16_t , uint16_t );
SYSCALL_FUNCTION syscalls[256] = {0};

void syscall_test(uint16_t num, uint16_t val) {
	printf("syscall (%d), val=%d\n", num,  val);
}

void initialize_syscalls() {
	for (int i = 0; i<256; i++)
		syscalls[i] = syscall_test;
}

void dispatch_syscall(uint16_t num, uint16_t val) {
	SYSCALL_FUNCTION func = syscalls[num];

	if (func)
		func(num, val);
}
