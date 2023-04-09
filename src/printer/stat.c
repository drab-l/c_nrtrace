#include <string_buffer.h>
#include <type_uni.h>
#include <print_uni.h>

#include "printer.h"
#include "syscall_info.h"
#include "time.h"
#include "stat.h"

#ifndef sb_bit_stat
#define sb_bit_stat(pr_, s_, ...) do { \
    sb_print_struct_top_member_hex((pr_)->sb, s_, st_dev); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, st_ino); \
    sb_print_struct_cnt_member_hex((pr_)->sb, s_, st_mode); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, st_nlink); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, st_uid); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, st_gid); \
    sb_print_struct_cnt_member_hex((pr_)->sb, s_, st_rdev); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, st_size); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, st_blksize); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, st_blocks); \
    string_buffer_strcat((pr_)->sb, ", .st_atim = {"); sb_print_timespec(pr_, (&s_->st_atim), sizeof(s_->st_atim)); \
    string_buffer_strcat((pr_)->sb, "}, .st_mtim = {"); sb_print_timespec(pr_, (&s_->st_mtim), sizeof(s_->st_mtim)); \
    string_buffer_strcat((pr_)->sb, "}, .st_ctim = {"); sb_print_timespec(pr_, (&s_->st_ctim), sizeof(s_->st_ctim)); \
    string_buffer_charcat((pr_)->sb, '}'); \
} while (0)
#endif

#ifndef sb_print_bit64_stat
static void sb_print_bit64_stat(printer *pr, bit64_stat *buf, size_t) { sb_bit_stat(pr, buf); }
#endif
#ifndef sb_print_bit32_stat
static void sb_print_bit32_stat(printer *pr, bit32_stat *buf, size_t) { sb_bit_stat(pr, buf); }
#endif

void sb_print_stat(printer *pr, void *buf, size_t size)
{
    IMPL_SB_PRINT_BIT_TYPE(stat, pr, buf, size);
}

#ifndef sb_bit_stat64
#define sb_bit_stat64(...) sb_bit_stat(__VA_ARGS__)
#endif

#ifndef sb_print_bit64_stat64
static void sb_print_bit64_stat64(printer *pr, bit64_stat64 *buf, size_t) { sb_bit_stat64(pr, buf); }
#endif

#ifndef sb_print_bit32_stat64
static void sb_print_bit32_stat64(printer *pr, bit32_stat64 *buf, size_t) { sb_bit_stat64(pr, buf); }
#endif

void sb_print_stat64(printer *pr, void *buf, size_t size)
{
    IMPL_SB_PRINT_BIT_TYPE(stat64, pr, buf, size);
}

void sb_print_statx_timestamp(printer *pr, void *buf, size_t)
{
    struct statx_timestamp *s = buf;
    sb_print_struct_top_member_dec(pr->sb, s, tv_sec);
    sb_print_struct_cnt_member_dec(pr->sb, s, tv_nsec);
}

void sb_print_statx(printer *pr, void *buf, size_t)
{
    struct statx *s = buf;
    sb_print_struct_top_member_hex(pr->sb, s, stx_mask);
    sb_print_struct_cnt_member_dec(pr->sb, s, stx_blksize);
    sb_print_struct_cnt_member_hex(pr->sb, s, stx_attributes);
    sb_print_struct_cnt_member_dec(pr->sb, s, stx_nlink);
    sb_print_struct_cnt_member_dec(pr->sb, s, stx_uid);
    sb_print_struct_cnt_member_dec(pr->sb, s, stx_gid);
    sb_print_struct_cnt_member_hex(pr->sb, s, stx_mode);
    sb_print_struct_cnt_member_dec(pr->sb, s, stx_ino);
    sb_print_struct_cnt_member_dec(pr->sb, s, stx_size);
    sb_print_struct_cnt_member_dec(pr->sb, s, stx_blocks);
    sb_print_struct_cnt_member_hex(pr->sb, s, stx_attributes_mask);

    string_buffer_strcat(pr->sb, ", .stx_atim = {"); sb_print_statx_timestamp(pr, (&s->stx_atime), sizeof(s->stx_atime));
    string_buffer_strcat(pr->sb, "}, .stx_ctim = {"); sb_print_statx_timestamp(pr, (&s->stx_btime), sizeof(s->stx_btime));
    string_buffer_strcat(pr->sb, "}, .stx_ctim = {"); sb_print_statx_timestamp(pr, (&s->stx_ctime), sizeof(s->stx_ctime));
    string_buffer_strcat(pr->sb, "}, .stx_mtim = {"); sb_print_statx_timestamp(pr, (&s->stx_mtime), sizeof(s->stx_mtime));
    string_buffer_charcat(pr->sb, '}');

    sb_print_struct_cnt_member_hex(pr->sb, s, stx_rdev_major);
    sb_print_struct_cnt_member_hex(pr->sb, s, stx_rdev_minor);
    sb_print_struct_cnt_member_hex(pr->sb, s, stx_dev_major);
    sb_print_struct_cnt_member_hex(pr->sb, s, stx_dev_minor);
}

