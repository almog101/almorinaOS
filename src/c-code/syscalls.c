#include <stdint.h>
#include <stdio.h>
#include "interrupts.h"

typedef void (*SYSCALL_FUNCTION) (void);
SYSCALL_FUNCTION syscalls[256];

extern int* get_stack_pointer(void);

void sys_test(void) 
{
	printf("\n>> test <<\n\n");
}

void sys_fork(void) {}
void sys_exit(void) {}
void sys_wait(void) {}
void sys_open(void) {}
void sys_read(void) {}
void sys_write(void) {}
void sys_close(void) {}
void sys_getpid(void) {}
void sys_alarm(void) {}
void sys_sleep(void) {}

void initialize_syscalls()
{
	syscalls[0] = sys_fork;
	syscalls[1] = sys_exit;
	syscalls[2] = sys_wait;
	syscalls[3] = sys_open;
	syscalls[4] = sys_read;
	syscalls[5] = sys_write;
	syscalls[6] = sys_close;
	syscalls[7] = sys_getpid;
	syscalls[8] = sys_alarm;
	syscalls[9] = sys_sleep;
	syscalls[10] = sys_test;

	for(int i = 11; i < 256; i++)
		syscalls[i] = 0;
}

void dispatch_syscall(uint16_t num, uint16_t val) 
{
	SYSCALL_FUNCTION func = syscalls[num];

	if (func)
		func();
}
