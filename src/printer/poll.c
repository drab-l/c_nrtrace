#include "poll.h"

#ifndef sb_bit_pollfd
#define sb_bit_pollfd(pr_, s_) do { \
    sb_print_struct_top_member_dec((pr_)->sb, s_, fd); \
    sb_print_struct_cnt_member_hex((pr_)->sb, s_, events); \
    sb_print_struct_cnt_member_hex((pr_)->sb, s_, revents); \
} while (0)
#endif

#ifndef sb_print_bit64_pollfd
static void sb_print_bit64_pollfd(printer *pr, bit64_pollfd *buf, size_t) { sb_bit_pollfd(pr, buf); }
#endif
#ifndef sb_print_bit32_pollfd
static void sb_print_bit32_pollfd(printer *pr, bit32_pollfd *buf, size_t) { sb_bit_pollfd(pr, buf); }
#endif

void sb_print_pollfd(printer *pr, void *buf, size_t size)
{
    IMPL_SB_PRINT_BIT_TYPE(pollfd, pr, buf, size);
}
