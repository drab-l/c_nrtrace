#pragma once

#include <string_buffer.h>
#include <type_uni.h>
#include <print_uni.h>

#include "printer.h"

#ifndef bit64_timespec
typedef struct { bit64_time_t tv_sec; bit64_slong tv_nsec; } bit64_timespec;
#endif
#ifndef bit32_timespec
typedef struct { bit32_time_t tv_sec; bit32_slong tv_nsec; } bit32_timespec;
#endif

void sb_print_timespec(printer *pr, void *buf, size_t size);

#define enum_table_clockid_size 8
