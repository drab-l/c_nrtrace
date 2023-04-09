#pragma once

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <linux/ptrace.h>
#include <unistd.h>

#include <util_header_all.h>
#include <type_uni.h>
#include "syscall_info.h"

typedef long peek_t;
#define PEEK_DATA_UNIT_BYTE (sizeof(peek_t))
typedef enum {PEEK_AGAIN, PEEK_ERROR, PEEK_FORKSTOP, PEEK_EXITED, PEEK_SYSSTOP} PEEK_EVENT_TYPE;

size_t peek_data(void *buf, size_t size, pid_t pid, bit_ptr_t addr);
int peek_syscall_info(syscall_info_t *si, pid_t pid);
void peek_cont_proc(pid_t pid);
pid_t peek_attach_exec_child(char *argv[]);
pid_t peek_attach_running_proc(pid_t pid);
void peek_treat_fork(pid_t parent);
PEEK_EVENT_TYPE peek_treat_wait_status(pid_t, int);

#define peek_trype(ret_, pid_, addr_) peek_data(&ret_, sizeof(ret_), pid_, addr_)

#define declarate_peek_number(type_) type_ peek_number_##type_(pid_t, bit_ptr_t)
declarate_peek_number(char);
declarate_peek_number(schar);
declarate_peek_number(uchar);
declarate_peek_number(sshort);
declarate_peek_number(ushort);
declarate_peek_number(sint);
declarate_peek_number(uint);
declarate_peek_number(slong);
declarate_peek_number(ulong);
declarate_peek_number(sllong);
declarate_peek_number(ullong);

#define peek_number(x_, pid_, addr_) _Generic((x_), char: peek_number_char(pid_, addr_), \
schar: peek_number_schar(pid_, addr_), uchar: peek_number_uchar(pid_, addr_), \
sshort: peek_number_sshort(pid_, addr_), ushort: peek_number_ushort(pid_, addr_), \
sint: peek_number_sint(pid_, addr_), uint: peek_number_uint(pid_, addr_), \
slong: peek_number_slong(pid_, addr_), ulong: peek_number_ulong(pid_, addr_), \
sllong: peek_number_sllong(pid_, addr_), ullong: peek_number_ullong(pid_, addr_))
