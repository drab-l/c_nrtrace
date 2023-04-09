#pragma once
#include <string_buffer.h>
#include <type_uni.h>
#include <print_uni.h>

#include "printer.h"

#ifndef bit64_iovec
typedef struct {bit64_ptr_t iov_base; bit64_size_t iov_len;} bit64_iovec;
#endif
#ifndef bit32_iovec
typedef struct {bit32_ptr_t iov_base; bit32_size_t iov_len;} bit32_iovec;
#endif

