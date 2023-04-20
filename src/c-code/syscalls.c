// #include <syscall.h>
// #include <stdint.h>
// #include <stdio.h>
// #include "interrupts.h"
// #include <string.h>
// #include <fs.h>

// typedef void (*SYSCALL_FUNCTION) (void);
// SYSCALL_FUNCTION syscalls[256];

// int64_t* rsp_register;

// void sys_fork(void) {}
// void sys_exit(void) {}
// void sys_open(void) {}

// void sys_read(void)
// {
// 	printf("syscall:\tread\n");

// 	int output = *(rsp_register);
// 	char* str = *(rsp_register + 1);
// 	int str_len = *(rsp_register + 2);
// 	printf("output:\t\t%d\nstring:\t\t%s\nstring length:\t%d\n\n", output, str, str_len);
// }

// void sys_write(void) 
// {
// 	printf("\nsyscall:\twrite\n");

// 	// fs_inode_t* output = *(rsp_register + 1);
// 	// char* str = *(rsp_register + 2);
// 	// int str_len = *(rsp_register + 3);

// 	if (1 == STDOUT)
// 	{
// 		// printf("data:\t\t%slength:\t\t%d\n", str, str_len);
// 		putc('\n');
// 	}
	
// 	else
// 	{
// 		// char* content;
// 		// strncpy(content, str, str_len);
// 		// fs_inode_write_data(ramfs_device, output, content);
// 		// printf("data:\t\t%slength:\t\t%d\n", output->blocks[0], str_len);
// 		// while (1) {}
// 	}
// }

// void sys_close(void) {}

// void initialize_syscalls()
// {
// 	syscalls[0] = 0;
// 	syscalls[1] = sys_exit;
// 	syscalls[2] = sys_fork;
// 	syscalls[3] = sys_read;
// 	syscalls[4] = sys_write;
// 	syscalls[5] = sys_open;
// 	syscalls[6] = sys_close;

// 	for(int i = 6; i < 256; i++)
// 		syscalls[i] = 0;
// }

// void dispatch_syscall(int64_t num) 
// {
// 	SYSCALL_FUNCTION func = syscalls[num];

// 	if (func)
// 		func();
// }
