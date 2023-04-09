#pragma once

#include <string_buffer.h>
#include <sys/types.h>
#include <syscall_info.h>

int printer_print_syscall(string_buffer *sb, syscall_info_t *);

typedef const struct {
    string_buffer *sb;
    syscall_info_t *si;
} printer;

