#include <syscall.h>
#include <stdio.h>

#define STD_OUTPUT 1

typedef int64_t (*SYSCALL_FUNCTION) (int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3);
SYSCALL_FUNCTION syscalls[256];

int64_t sys_read(int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3)
{
	printf("syscall:\tread\n");

	return arg1;
}

int64_t sys_write(int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3) 
{
	printf("syscall:\twrite\n");

	if(arg0 == STD_OUTPUT)
	{
		printf(arg1);
		putc('\n');
	}
	else 
	{
		arg0 = arg1;
	}

	return 0;
}

int64_t sys_open(int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3) { return 0; }
int64_t sys_close(int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3) { return 0; }

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

int64_t dispatch_syscall(int64_t num, int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3) 
{
	SYSCALL_FUNCTION func = syscalls[num];

	if (func)
		return func(arg0, arg1, arg2, arg3);

	else
		return 0;
}
