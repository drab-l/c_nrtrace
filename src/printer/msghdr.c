#include <util_header_all.h>
#include <peek.h>

#include "msghdr.h"

#define cmsg_align_(l_,c_) (((l_) + sizeof(c_->cmsg_len) - 1) & ~(sizeof(c_->cmsg_len) - 1))
#define cmsg_next_(c_) ((typeof(c_))((uintptr_t)(c_) + cmsg_align_(c->cmsg_len, c)))
#define cmsg_nxthdr_in_(h_, m_, c_) do { \
    if ((c_)->cmsg_len < sizeof(typeof(*(c_)))) { c_ = NULL; break; } \
    c_ = cmsg_next_(c_); \
    if ((uintptr_t) (c_ + 1) > (uintptr_t)(m_ + (h_)->msg_controllen)) { c_ = NULL; } \
    else if ((uintptr_t)cmsg_next_(c_) > (uintptr_t)(m_ + (h_)->msg_controllen)) { c_ = NULL; } \
} while (0)

static bit64_cmsghdr *cmsg_nxthdr_bit64(const bit64_msghdr *h, char *m, bit64_cmsghdr *c) {cmsg_nxthdr_in_(h, m, c); return c;}
static bit32_cmsghdr *cmsg_nxthdr_bit32(const bit32_msghdr *h, char *m, bit32_cmsghdr *c) {cmsg_nxthdr_in_(h, m, c); return c;}

#define cmsg_nxthdr_(si_, m_, c_) (_Generic((c_), bit64_cmsghdr* : cmsg_nxthdr_bit64, bit32_cmsghdr* : cmsg_nxthdr_bit32)(si_, m_, c_))

#define sb_bit_cmsg(pr_, s_) do { \
            sb_print_struct_top_member_dec((pr_)->sb, s_, cmsg_len); \
            sb_print_struct_cnt_member_dec((pr_)->sb, s_, cmsg_level); \
            sb_print_struct_cnt_member_dec((pr_)->sb, s_, cmsg_type); \
            string_buffer_strcat((pr_)->sb, ", cmsg_data = "); \
            const unsigned char *d = s_->cmsg_data; \
            const size_t s = s_->cmsg_len - pointer_diff(d, s_); \
            string_buffer_hexcat((pr_)->sb, d, s); \
} while (0)

#define type_select_(msg_, type_) typeof(*(_Generic(msg_, bit64_msghdr*: (bit64_##type_*)NULL, bit32_msghdr*: (bit32_##type_*)NULL)))

#define sb_bit_msghdr(pr_, msg_) do { \
    typeof(msg_) hdr = msg_; \
    char msg_name[hdr->msg_namelen]; \
    if (!hdr->msg_name || !hdr->msg_namelen || peek_data(msg_name, sizeof(msg_name), nrsi_pid((pr_)->si), (bit_ptr_t)hdr->msg_name)) { \
        if (!hdr->msg_name) { \
            string_buffer_strcat((pr_)->sb, ".msg_name = NULL"); \
        }  else { \
            string_buffer_strcat((pr_)->sb, ".msg_name = \"\""); \
        } \
    }  else { \
        string_buffer_strcat((pr_)->sb, ".msg_name = \""); \
        string_buffer_strcat((pr_)->sb, msg_name); \
        string_buffer_charcat((pr_)->sb, '"'); \
    } \
    sb_print_struct_cnt_member_dec((pr_)->sb, hdr, msg_namelen); \
    if (!hdr->msg_iov || !hdr->msg_iovlen) { \
        sb_print_struct_top_member_hex((pr_)->sb, hdr, msg_iov); \
        sb_print_struct_cnt_member_hex((pr_)->sb, hdr, msg_iovlen); \
        string_buffer_strcat((pr_)->sb, ", "); \
    } else { \
        string_buffer_strcat((pr_)->sb, ", .msg_iov = "); \
        peek_sb_print_array_callback_default(pr_, hdr->msg_iov, hdr->msg_iovlen, iovec, sizeof(type_select_(msg_, iovec))); \
        sb_print_struct_cnt_member_dec((pr_)->sb, hdr, msg_iovlen); \
    } \
    _Alignas(_Alignof(type_select_(msg_, cmsghdr))) char msg_control[hdr->msg_controllen]; \
    if (!hdr->msg_control || hdr->msg_controllen < sizeof(type_select_(msg_, cmsghdr)) || peek_data(msg_control, sizeof(msg_control), nrsi_pid((pr_)->si), (bit_ptr_t)hdr->msg_control)) { \
        sb_print_struct_top_member_hex((pr_)->sb, hdr, msg_control); \
        string_buffer_strcat((pr_)->sb, ", "); \
    } else { \
        string_buffer_strcat((pr_)->sb, ".msg_control = {\n"); \
        for (type_select_(msg_, cmsghdr) *cmsg = (type_select_(msg_, cmsghdr)*)msg_control; cmsg; cmsg = cmsg_nxthdr_(hdr, msg_control, cmsg)) { \
            string_buffer_strcat((pr_)->sb, "\t{"); \
            sb_bit_cmsg(pr_, cmsg); \
            string_buffer_strcat((pr_)->sb, "},\n"); \
        } \
        string_buffer_strcat((pr_)->sb, "\t}, "); \
    } \
    sb_print_struct_top_member_hex((pr_)->sb, hdr, msg_controllen); \
    sb_print_struct_cnt_member_hex((pr_)->sb, hdr, msg_flags); \
} while (0)

#ifndef sb_print_bit64_msghdr
static void sb_print_bit64_msghdr(printer *pr, bit64_msghdr *buf, size_t) { sb_bit_msghdr(pr, buf); }
#endif

#ifndef sb_print_bit32_msghdr
static void sb_print_bit32_msghdr(printer *pr, bit32_msghdr *buf, size_t) { sb_bit_msghdr(pr, buf); }
#endif

void sb_print_msghdr(printer *pr, void *buf, size_t size)
{
    IMPL_SB_PRINT_BIT_TYPE(msghdr, pr, buf, size);
}

