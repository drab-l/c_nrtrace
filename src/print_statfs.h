#pragma once
#include "print/macro.h"

#define sb_bit_statfs(sb_, s_) do { \
    string_buffer_charcat(sb_, '{'); \
    sb_print_struct_top_member_hex(sb_, s_, f_type); \
    sb_print_struct_cnt_member_dec(sb_, s_, f_bsize); \
    sb_print_struct_cnt_member_dec(sb_, s_, f_blocks); \
    sb_print_struct_cnt_member_dec(sb_, s_, f_bfree); \
    sb_print_struct_cnt_member_dec(sb_, s_, f_bavail); \
    sb_print_struct_cnt_member_dec(sb_, s_, f_files); \
    sb_print_struct_cnt_member_dec(sb_, s_, f_ffree); \
    string_buffer_strcat(sb_, ", .f_fsid = {"); \
    string_buffer_dec(sb_, s_->f_fsid.__val[0]); \
    string_buffer_charcat(sb_, ','); \
    string_buffer_dec(sb_, s_->f_fsid.__val[1]); \
    string_buffer_charcat(sb_, '}'); \
    sb_print_struct_cnt_member_dec(sb_, s_, f_namelen); \
    sb_print_struct_cnt_member_dec(sb_, s_, f_frsize); \
    sb_print_struct_cnt_member_hex(sb_, s_, f_flags); \
    string_buffer_charcat(sb_, '}'); \
} while (0)

#ifndef bit64_statfs
typedef struct { uint64_t f_type; uint64_t f_bsize; uint64_t f_blocks; uint64_t f_bfree; uint64_t f_bavail; uint64_t f_files; uint64_t f_ffree; fsid_t f_fsid; uint64_t f_namelen; uint64_t f_frsize; uint64_t f_flags; uint64_t f_spare[4]; } bit64_statfs;
#endif
#ifndef sb_bit64_statfs
DEF_DEFAULT_SB_PRINT(64, statfs)
#endif
#ifndef bit32_statfs
typedef struct { uint32_t f_type; uint32_t f_bsize; uint32_t f_blocks; uint32_t f_bfree; uint32_t f_bavail; uint32_t f_files; uint32_t f_ffree; fsid_t f_fsid; uint32_t f_namelen; uint32_t f_frsize; uint32_t f_flags; uint32_t f_spare[4]; } bit32_statfs;
#endif
#ifndef sb_bit32_statfs
DEF_DEFAULT_SB_PRINT(32, statfs)
#endif
DEF_PRINT_PTR_SB_DUMP(statfs)

#ifndef sb_bit_statfs64
#define sb_bit_statfs64 sb_bit_statfs
#endif

#ifndef bit64_statfs64
typedef struct { uint64_t f_type; uint64_t f_bsize; uint64_t f_blocks; uint64_t f_bfree; uint64_t f_bavail; uint64_t f_files; uint64_t f_ffree; fsid_t f_fsid; uint64_t f_namelen; uint64_t f_frsize; uint64_t f_flags; uint64_t f_spare[4]; } bit64_statfs64;
#endif
#ifndef sb_bit64_statfs64
DEF_DEFAULT_SB_PRINT(64, statfs64)
#endif
#ifndef bit32_statfs64
typedef struct { uint32_t f_type; uint32_t f_bsize; uint64_t f_blocks; uint64_t f_bfree; uint64_t f_bavail; uint64_t f_files; uint64_t f_ffree; fsid_t f_fsid; uint32_t f_namelen; uint32_t f_frsize; uint32_t f_flags; uint32_t f_spare[4]; } bit32_statfs64;
#endif
#ifndef sb_bit32_statfs64
DEF_DEFAULT_SB_PRINT(32, statfs64)
#endif
DEF_PRINT_PTR_SB_DUMP(statfs64)
