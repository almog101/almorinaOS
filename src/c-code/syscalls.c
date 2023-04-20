#include <syscall.h>
#include <stdio.h>

typedef void (*SYSCALL_FUNCTION) (int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3);
SYSCALL_FUNCTION syscalls[256];

void sys_read(int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3)
{
	// printf("syscall:\tread\n");
}

void sys_write(int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3) {}
void sys_open(int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3) {}
void sys_close(int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3) {}

// THIS WHOLE DOCUMENT IS CURSED

void initialize_syscalls()
{
	syscalls[0] = 0;
	syscalls[1] = sys_read;
	syscalls[2] = sys_write;
	syscalls[3] = sys_open;
	syscalls[4] = sys_close;

	for(int i = 5; i < 256; i++)
		syscalls[i] = 0;
}

void dispatch_syscall(int64_t num, int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3) 
{
	printf("hello\n");
	SYSCALL_FUNCTION func = syscalls[num];

	if (func)
		func(arg0, arg1, arg2, arg3);
}
