#pragma once
#include <sys/types.h>
#include <string_buffer.h>
#include <type_uni.h>
#include <print_uni.h>

#include "printer.h"
#include "time.h"

#ifndef bit64_stat
typedef struct { bit64_dev_t st_dev; bit64_ino_t st_ino; bit64_mode_t st_mode; bit64_nlink_t st_nlink; bit64_uid_t st_uid; bit64_gid_t st_gid; bit64_dev_t st_rdev; bit64_ulong pad1; bit64_off_t st_size; bit64_sint st_blksize; bit64_sint pad2; bit64_slong st_blocks; bit64_timespec st_atim; bit64_timespec  st_mtim; bit64_timespec st_ctim; bit64_uint unused4; bit64_uint unused5; } bit64_stat;
#endif
#ifndef bit32_stat
typedef struct { bit32_dev_t st_dev; bit32_uchar pad0[4]; bit32_ino_t st_ino__; bit32_mode_t st_mode; bit32_nlink_t st_nlink; bit32_uid_t st_uid; bit32_gid_t st_gid; bit32_dev_t st_rdev; bit32_uchar pad3[4]; bit32_sllong st_size; bit32_ulong st_blksize; bit32_ullong st_blocks; bit32_timespec st_atim; bit32_timespec st_mtim; bit32_timespec st_ctim; bit32_ullong st_ino; } bit32_stat;
#endif


#ifndef bit64_stat64
typedef struct { bit64_dev_t st_dev; bit64_ino_t st_ino; bit64_mode_t st_mode; bit64_nlink_t st_nlink; bit64_uid_t st_uid; bit64_gid_t st_gid; bit64_dev_t st_rdev; bit64_ulong pad1; bit64_off_t st_size; bit64_sint st_blksize; bit64_sint pad2; bit64_slong st_blocks; bit64_timespec st_atim; bit64_timespec  st_mtim; bit64_timespec st_ctim; bit64_uint unused4; bit64_uint unused5; } bit64_stat64;
#endif
#ifndef bit32_stat64
typedef struct { bit32_dev_t st_dev; bit32_uchar pad0[4]; bit32_ino_t st_ino_; bit32_mode_t st_mode; bit32_nlink_t st_nlink; bit32_uid_t st_uid; bit32_gid_t st_gid; bit32_dev_t st_rdev; bit32_uchar pad3[4]; bit32_sllong st_size; bit32_ulong st_blksize; bit32_ullong st_blocks; bit32_timespec st_atim; bit32_timespec st_mtim; bit32_timespec st_ctim; bit32_ullong st_ino; } bit32_stat64;
#endif

struct statx_timestamp {
    int64_t tv_sec;
    uint32_t tv_nsec;
};

struct statx {
    uint32_t stx_mask;
    uint32_t stx_blksize;
    uint64_t stx_attributes;
    uint32_t stx_nlink;
    uint32_t stx_uid;
    uint32_t stx_gid;
    uint16_t stx_mode;
    uint64_t stx_ino;
    uint64_t stx_size;
    uint64_t stx_blocks;
    uint64_t stx_attributes_mask;
    struct statx_timestamp stx_atime;
    struct statx_timestamp stx_btime;
    struct statx_timestamp stx_ctime;
    struct statx_timestamp stx_mtime;
    uint32_t stx_rdev_major;
    uint32_t stx_rdev_minor;
    uint32_t stx_dev_major;
    uint32_t stx_dev_minor;
};
