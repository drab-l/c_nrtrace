#include "epoll.h"

#ifndef sb_bit_epoll_event
#define sb_bit_epoll_event(pr_, s_) do { \
    sb_print_struct_top_member_dec((pr_)->sb, s_, events); \
    string_buffer_strcat((pr_)->sb, ", .data = "); \
    string_buffer_hex((pr_)->sb, s_->u64); \
} while (0)
#endif

#ifndef sb_print_bit64_epoll_event
static void sb_print_bit64_epoll_event(printer *pr, bit64_epoll_event *buf, size_t) { sb_bit_epoll_event(pr, buf); }
#endif
#ifndef sb_print_bit32_epoll_event
static void sb_print_bit32_epoll_event(printer *pr, bit32_epoll_event *buf, size_t) { sb_bit_epoll_event(pr, buf); }
#endif

void sb_print_epoll_event(printer *pr, void *buf, size_t size)
{
    IMPL_SB_PRINT_BIT_TYPE(epoll_event, pr, buf, size);
}
