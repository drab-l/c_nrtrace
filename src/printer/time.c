#include <time.h>
#include <util_header_all.h>

#include "time.h"

#ifndef sb_bit_timespec
#define sb_bit_timespec(pr_, s_,...) do { \
    sb_print_struct_top_member_dec((pr_)->sb, s_, tv_sec); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, tv_nsec); \
} while (0)
#endif

#ifndef sb_print_bit64_timespec
static void sb_print_bit64_timespec(printer *pr, bit64_timespec *buf, size_t) { sb_bit_timespec(pr, buf); }
#endif
#ifndef sb_print_bit324_timespec
static void sb_print_bit32_timespec(printer *pr, bit32_timespec *buf, size_t) { sb_bit_timespec(pr, buf); }
#endif

void sb_print_timespec(printer *pr, void *buf, size_t size)
{
    IMPL_SB_PRINT_BIT_TYPE(timespec, pr, buf, size);
}

#define define_enum_table_elem(n_) {n_, #n_}
const enum_table enum_table_clockid[] = {
    define_enum_table_elem(CLOCK_REALTIME),
    define_enum_table_elem(CLOCK_REALTIME_COARSE),
    define_enum_table_elem(CLOCK_MONOTONIC),
    define_enum_table_elem(CLOCK_MONOTONIC_COARSE),
    define_enum_table_elem(CLOCK_MONOTONIC_RAW),
    define_enum_table_elem(CLOCK_BOOTTIME),
    define_enum_table_elem(CLOCK_PROCESS_CPUTIME_ID),
    define_enum_table_elem(CLOCK_THREAD_CPUTIME_ID)
};

