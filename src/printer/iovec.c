#include "iovec.h"
#include <bits/posix1_lim.h>

#ifndef sb_bit_iovec
#define sb_bit_iovec(pr_, s_) do { \
    string_buffer_strcat(pr_->sb, ".iov_base = "); \
    typeof(s_->iov_len) iov_len = ((uintptr_t)pr->data < s_->iov_len) ? (uintptr_t)pr->data : s_->iov_len; \
    printer_peek_sb_print_graph_ascii_or_hex(pr, s_->iov_base, iov_len); \
    pr->data = (void*)((uintptr_t)pr->data - iov_len); \
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

