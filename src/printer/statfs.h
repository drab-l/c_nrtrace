#pragma once
#include <string_buffer.h>
#include <type_uni.h>
#include <print_uni.h>

#include "printer.h"

#ifndef bit64_statfs
typedef struct { uint64_t f_type; uint64_t f_bsize; uint64_t f_blocks; uint64_t f_bfree; uint64_t f_bavail; uint64_t f_files; uint64_t f_ffree; fsid_t f_fsid; uint64_t f_namelen; uint64_t f_frsize; uint64_t f_flags; uint64_t f_spare[4]; } bit64_statfs;
#endif
#ifndef bit32_statfs
typedef struct { uint32_t f_type; uint32_t f_bsize; uint32_t f_blocks; uint32_t f_bfree; uint32_t f_bavail; uint32_t f_files; uint32_t f_ffree; fsid_t f_fsid; uint32_t f_namelen; uint32_t f_frsize; uint32_t f_flags; uint32_t f_spare[4]; } bit32_statfs;
#endif

#ifndef bit64_statfs64
typedef struct { uint64_t f_type; uint64_t f_bsize; uint64_t f_blocks; uint64_t f_bfree; uint64_t f_bavail; uint64_t f_files; uint64_t f_ffree; fsid_t f_fsid; uint64_t f_namelen; uint64_t f_frsize; uint64_t f_flags; uint64_t f_spare[4]; } bit64_statfs64;
#endif
#ifndef bit32_statfs64
typedef struct { uint32_t f_type; uint32_t f_bsize; uint64_t f_blocks; uint64_t f_bfree; uint64_t f_bavail; uint64_t f_files; uint64_t f_ffree; fsid_t f_fsid; uint32_t f_namelen; uint32_t f_frsize; uint32_t f_flags; uint32_t f_spare[4]; } bit32_statfs64;
#endif
