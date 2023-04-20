#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

void initialize_syscalls();
void dispatch_syscall(int64_t num, int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3);

#endif