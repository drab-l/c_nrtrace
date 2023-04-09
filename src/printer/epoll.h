#pragma once
#include <string_buffer.h>
#include <type_uni.h>
#include <print_uni.h>

#include "printer.h"

#ifndef bit64_epoll_event
typedef struct { uint32_t events; union { bit64_ptr_t ptr; bit64_sint fd; uint32_t u32; uint64_t u64; }; } bit64_epoll_event;
#endif
#ifndef bit32_epoll_event
typedef struct { uint32_t events; union { bit32_ptr_t ptr; bit32_sint fd; uint32_t u32; uint64_t u64; }; } bit32_epoll_event;
#endif
