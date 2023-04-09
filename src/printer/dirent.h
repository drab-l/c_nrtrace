#pragma once
#include <string_buffer.h>
#include <type_uni.h>
#include <print_uni.h>

#include "printer.h"

#ifndef bit64_linux_dirent64
typedef struct {uint64_t d_ino; uint64_t d_off; uint16_t d_reclen; char d_type; char d_name[];} bit64_linux_dirent64;
#endif
#ifndef bit32_linux_dirent64
typedef struct {uint64_t d_ino; uint64_t d_off; uint16_t d_reclen; char d_type; char d_name[];} bit32_linux_dirent64;
#endif

#ifndef bit64_linux_dirent
typedef struct {uint64_t d_ino; uint64_t d_off; uint16_t d_reclen; char d_name[];} bit64_linux_dirent;
#endif
#ifndef bit32_linux_dirent
typedef struct {uint32_t d_ino; uint32_t d_off; uint16_t d_reclen; char d_name[];} bit32_linux_dirent;
#endif

