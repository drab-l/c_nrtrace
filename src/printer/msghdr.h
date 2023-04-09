#pragma once
#include <sys/resource.h>
#include <string_buffer.h>
#include <type_uni.h>
#include <print_uni.h>

#include "printer.h"
#include "iovec.h"

#ifndef bit64_msghdr
typedef struct {bit64_ptr_t msg_name; uint32_t msg_namelen; bit64_ptr_t msg_iov; bit64_size_t msg_iovlen; bit64_ptr_t msg_control; bit64_size_t msg_controllen; int32_t msg_flags;} bit64_msghdr;
typedef struct {bit64_size_t cmsg_len; int32_t cmsg_level; int32_t cmsg_type; unsigned char cmsg_data[];} bit64_cmsghdr;
#endif
#ifndef bit32_msghdr
typedef struct {bit32_ptr_t msg_name; uint32_t msg_namelen; bit32_ptr_t msg_iov; bit32_size_t msg_iovlen; bit32_ptr_t msg_control; bit32_size_t msg_controllen; int32_t msg_flags;} bit32_msghdr;
typedef struct {bit32_size_t cmsg_len; int32_t cmsg_level; int32_t cmsg_type; unsigned char cmsg_data[];} bit32_cmsghdr;
#endif

