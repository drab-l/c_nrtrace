#include <util_header_all.h>
#include <peek.h>

#include "msghdr.h"

#define cmsg_align_(l_,c_) (((l_) + sizeof(c_->cmsg_len) - 1) & ~(sizeof(c_->cmsg_len) - 1))
#define cmsg_next_(c_) ((typeof(c_))((uintptr_t)(c_) + cmsg_align_(c->cmsg_len, c)))
#define cmsg_nxthdr_in_(h_, m_, c_) do { \
    if ((c_)->cmsg_len < sizeof(typeof(*(c_)))) { return 0; } \
    c_ = cmsg_next_(c_); \
    if ((uintptr_t) (c_ + 1) > (uintptr_t)(m_ + (h_)->msg_controllen)) { c_ = NULL; } \
    else if ((uintptr_t)cmsg_next_(c_) > (uintptr_t)(m_ + (h_)->msg_controllen)) { c_ = NULL; } \
} while (0)

#define DEF_CMSGNXTHDR_FUNC(b_) static bit##b_##_cmsghdr *cmsg_nxthdr_##b_(const bit##b_##_msghdr *h, char *m, bit##b_##_cmsghdr *c) {cmsg_nxthdr_in_(h, m, c); return c;}
DEF_CMSGNXTHDR_FUNC(32)
DEF_CMSGNXTHDR_FUNC(64)
#define cmsg_nxthdr_(si_, m_, c_) (_Generic((c_), bit64_cmsghdr* : cmsg_nxthdr_64, bit32_cmsghdr* : cmsg_nxthdr_32)(si_, m_, c_))
#define PRINT_PTR2MSGHDR(t_, sb_, si_, buf_) do { \
    typeof(*(t_)->h) *hdr = buf_; \
    char msg_name[hdr->msg_namelen]; \
    if (!hdr->msg_name || !hdr->msg_namelen || peek_data(msg_name, sizeof(msg_name), nrsi_pid(si_), (bit_ptr_t)hdr->msg_name)) { \
        msg_name[0] = '\0'; \
    } \
    string_buffer_strcat(sb_, "{.msg_name = \""); string_buffer_strcat(sb_, msg_name); \
    string_buffer_strcat(sb_, "\", .msg_namelen = "); string_buffer_dec(sb_, hdr->msg_namelen); \
    typeof(*(t_)->i) msg_iov[hdr->msg_iovlen]; \
    if (!hdr->msg_iov || !hdr->msg_iovlen || peek_data(msg_iov, sizeof(msg_iov), nrsi_pid(si_), (bit_ptr_t)hdr->msg_iov)) { \
        sb_print_struct_top_member_hex(sb_, hdr, msg_iov); \
        sb_print_struct_cnt_member_hex(sb_, hdr, msg_iovlen); \
        string_buffer_strcat(sb_, ", "); \
    } else { \
        string_buffer_strcat(sb_, ", .msg_iov = {\n"); \
        for (size_t i = 0; i < hdr->msg_iovlen; i++) { \
            unsigned char b[msg_iov[i].iov_len]; \
            if (peek_data(b, sizeof(b), nrsi_pid(si_), (bit_ptr_t)msg_iov[i].iov_base)) { \
                string_buffer_strcat(sb_, "\t{"); \
                sb_print_struct_top_member_hex(sb_, (&msg_iov[i]), iov_base); \
            } else { \
                string_buffer_strcat(sb_, "\t{.iov_base = "); \
                string_buffer_hexcat(sb_, b, sizeof(b)); \
            } \
            sb_print_struct_cnt_member_dec(sb_, (&msg_iov[i]), iov_len); \
            string_buffer_strcat(sb_, "},\n"); \
        } \
        string_buffer_strcat(sb_, "\t}, "); \
    } \
    _Alignas(_Alignof(typeof(*(t_)->c))) char msg_control[hdr->msg_controllen]; \
    if (!hdr->msg_control || hdr->msg_controllen < sizeof(*(t_)->c) || peek_data(msg_control, sizeof(msg_control), nrsi_pid(si_), (bit_ptr_t)hdr->msg_control)) { \
        sb_print_struct_top_member_hex(sb_, hdr, msg_control); \
        string_buffer_strcat(sb_, ", "); \
    } else { \
        string_buffer_strcat(sb_, ".msg_control = {\n"); \
        for (typeof((t_)->c) cmsg = (typeof((t_)->c))msg_control; cmsg; cmsg = cmsg_nxthdr_(hdr, msg_control, cmsg)) { \
            string_buffer_strcat(sb_, "\t{"); \
            sb_print_struct_top_member_dec(sb_, cmsg, cmsg_len); \
            sb_print_struct_top_member_dec(sb_, cmsg, cmsg_level); \
            sb_print_struct_top_member_dec(sb_, cmsg, cmsg_type); \
            string_buffer_strcat(sb_, ", cmsg_data = "); \
            const unsigned char *d = cmsg->cmsg_data; \
            const size_t s = cmsg->cmsg_len - pointer_diff(d, cmsg); \
            string_buffer_hexcat(sb_, d, s); \
            string_buffer_strcat(sb_, "},\n"); \
        } \
        string_buffer_strcat(sb_, "\t}, "); \
    } \
    sb_print_struct_top_member_hex(sb_, hdr, msg_controllen); \
    sb_print_struct_cnt_member_hex(sb_, hdr, msg_flags); \
    string_buffer_charcat(sb_, '}'); \
} while(0)

void sb_print_msghdr(printer *pr, void *buf, size_t)
{
    string_buffer *sb = pr->sb;
    const syscall_info_t *si = pr->si;
    if (nrsi_is_bit64(si)) {
        PRINT_PTR2MSGHDR(&(struct {bit64_msghdr *h; bit64_iovec *i; bit64_cmsghdr *c;}){}, sb, si, buf);
    } else {
        PRINT_PTR2MSGHDR(&(struct {bit32_msghdr *h; bit32_iovec *i; bit32_cmsghdr *c;}){} , sb, si, buf);
    }
}

