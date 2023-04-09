#pragma once
#include <sys/resource.h>
#include <string_buffer.h>
#include <type_uni.h>
#include <print_uni.h>

#include "printer.h"

#ifndef bit64_rlimit
typedef struct { uint64_t rlim_cur; uint64_t rlim_max; } bit64_rlimit;
#endif
#ifndef bit32_rlimit
typedef struct { uint32_t rlim_cur; uint32_t rlim_max; } bit32_rlimit;
#endif

