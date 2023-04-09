#include "statfs.h"

#ifndef sb_bit_statfs
#define sb_bit_statfs(pr_, s_) do { \
    sb_print_struct_top_member_hex((pr_)->sb, s_, f_type); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, f_bsize); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, f_blocks); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, f_bfree); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, f_bavail); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, f_files); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, f_ffree); \
    string_buffer_strcat((pr_)->sb, ", .f_fsid = {"); \
    string_buffer_dec((pr_)->sb, s_->f_fsid.__val[0]); \
    string_buffer_charcat((pr_)->sb, ','); \
    string_buffer_dec((pr_)->sb, s_->f_fsid.__val[1]); \
    string_buffer_charcat((pr_)->sb, '}'); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, f_namelen); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, f_frsize); \
    sb_print_struct_cnt_member_hex((pr_)->sb, s_, f_flags); \
} while (0)
#endif

#ifndef sb_print_bit64_statfs
static void sb_print_bit64_statfs(printer *pr, bit64_statfs *buf, size_t) { sb_bit_statfs(pr, buf); }
#endif
#ifndef sb_print_bit32_statfs
static void sb_print_bit32_statfs(printer *pr, bit32_statfs *buf, size_t) { sb_bit_statfs(pr, buf); }
#endif

void sb_print_statfs(printer *pr, void *buf, size_t size)
{
    IMPL_SB_PRINT_BIT_TYPE(statfs, pr, buf, size);
}

#ifndef sb_bit_statfs64
#define sb_bit_statfs64 sb_bit_statfs
#endif

#ifndef sb_print_bit64_statfs64
static void sb_print_bit64_statfs64(printer *pr, bit64_statfs64 *buf, size_t) { sb_bit_statfs64(pr, buf); }
#endif
#ifndef sb_print_bit32_statfs64
static void sb_print_bit32_statfs64(printer *pr, bit32_statfs64 *buf, size_t) { sb_bit_statfs64(pr, buf); }
#endif

void sb_print_statfs64(printer *pr, void *buf, size_t size)
{
    IMPL_SB_PRINT_BIT_TYPE(statfs64, pr, buf, size);
}

