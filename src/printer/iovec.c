#include "iovec.h"
#include <bits/posix1_lim.h>

#ifndef sb_bit_iovec
#define sb_bit_iovec(pr_, s_) do { \
    string_buffer_strcat(pr_->sb, ".iov_base = "); \
    printer_peek_sb_print_graph_ascii_or_hex(pr, s_->iov_base, s_->iov_len); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, iov_len); \
} while (0)
#endif

#ifndef sb_bit_iovec_sized
#define sb_bit_iovec_sized(pr_, s_, r_) do { \
    string_buffer_strcat(pr_->sb, ".iov_base = "); \
    typeof(s_->iov_len) iov_len = (r_ < s_->iov_len) ? r_ : s_->iov_len; \
    printer_peek_sb_print_graph_ascii_or_hex(pr, s_->iov_base, iov_len); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, iov_len); \
    return iov_len; \
} while (0)
#endif

#ifndef sb_print_bit64_iovec
static void sb_print_bit64_iovec(printer *pr, bit64_iovec *buf, size_t) { sb_bit_iovec(pr, buf); }
#endif
#ifndef sb_print_bit32_iovec
static void sb_print_bit32_iovec(printer *pr, bit32_iovec *buf, size_t) { sb_bit_iovec(pr, buf); }
#endif

void sb_print_iovec(printer *pr, void *buf, size_t size)
{
    IMPL_SB_PRINT_BIT_TYPE(iovec, pr, buf, size);
}

typedef bit64_iovec bit64_iovec_sized;
typedef bit32_iovec bit32_iovec_sized;

#ifndef sb_print_bit64_iovec_sized
static size_t sb_print_bit64_iovec_sized(printer *pr, bit64_iovec_sized *buf, size_t remain) { sb_bit_iovec_sized(pr, buf, remain); }
#endif
#ifndef sb_print_bit32_iovec_sized
static size_t sb_print_bit32_iovec_sized(printer *pr, bit32_iovec_sized *buf, size_t remain) { sb_bit_iovec_sized(pr, buf, remain); }
#endif

void sb_print_iovec_array_sized_argR(printer *pr, void *buf, size_t)
{
    char *s = buf;
    size_t remain = nrsi_argR(pr->si);
    string_buffer_charcat(pr->sb, '{');
    while (1) {
        size_t r = IMPL_SB_PRINT_BIT_TYPE(iovec_sized, pr, s, remain);
        remain -= remain < r ? remain : r;
        s += bit_sizeof(pr, iovec_sized);
        if (remain > 0) {
            string_buffer_strcat(pr->sb, "}, {");
        } else {
            string_buffer_charcat(pr->sb, '}');
            break;
        }
    }
}

