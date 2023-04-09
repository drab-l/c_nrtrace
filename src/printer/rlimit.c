#include "rlimit.h"

#ifndef sb_bit_rlimit
#define sb_bit_rlimit(pr_, s_) do { \
    sb_print_struct_top_member_dec((pr_)->sb, s_, rlim_cur); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, rlim_max); \
} while (0)
#endif

#ifndef sb_print_bit64_rlimit
static void sb_print_bit64_rlimit(printer *pr, bit64_rlimit *buf, size_t) { sb_bit_rlimit(pr, buf); }
#endif
#ifndef sb_print_bit32_rlimit
static void sb_print_bit32_rlimit(printer *pr, bit32_rlimit *buf, size_t) { sb_bit_rlimit(pr, buf); }
#endif

void sb_print_rlimit(printer *pr, void *buf, size_t size)
{
    IMPL_SB_PRINT_BIT_TYPE(rlimit, pr, buf, size);
}

void sb_print_rlimit64(printer *pr, void *buf, size_t)
{
    struct rlimit64 *b = buf;
    sb_bit_rlimit(pr, b);
}

