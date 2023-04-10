#pragma once

#include <string_buffer.h>
#include <sys/types.h>
#include <syscall_info.h>

int printer_print_syscall(string_buffer *sb, syscall_info_t *);

typedef struct {
    string_buffer * const sb;
    const syscall_info_t * const si;
    void *data;
} printer;

