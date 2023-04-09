#include <string_buffer.h>
#include <type_uni.h>
#include <print_uni.h>

#include "printer.h"
#include "dirent.h"

#ifndef sb_bit_linux_dirent64
#define sb_bit_linux_dirent64(pr_, s_) do { \
    sb_print_struct_top_member_hex_zero((pr_)->sb, s_, d_ino); \
    sb_print_struct_cnt_member_hex_zero((pr_)->sb, s_, d_off); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, d_reclen); \
    sb_print_struct_cnt_member_hex_zero((pr_)->sb, s_, d_type); \
    string_buffer_strcat((pr_)->sb, ", .d_name = \""); \
    string_buffer_strcat((pr_)->sb, s_->d_name); \
} while (0)
#endif

void sb_print_array_prefix_linux_dirent64(printer *pr) { sb_print_array_prefix_default(pr); }
void sb_print_array_suffix_linux_dirent64(printer *pr) { sb_print_array_suffix_default(pr); }
void sb_print_elem_delim_linux_dirent64(printer *pr) { sb_print_elem_delim_default(pr); }
size_t sb_print_elem_size_linux_dirent64(printer *pr, void *buf, size_t size) {
    if (nrsi_is_bit64(pr->si)) {
        bit64_linux_dirent64 *base = buf;
        if (size < sizeof(*base) || size < base->d_reclen) {
            return size;
        }
        return base->d_reclen;
    } else {
        bit32_linux_dirent64 *base = buf;
        if (size < sizeof(*base) || size < base->d_reclen) {
            return size;
        }
        return base->d_reclen;
    }
}

void sb_print_linux_dirent64(printer *pr, void *buf, size_t size)
{
    if (nrsi_is_bit64(pr->si)) {
        bit64_linux_dirent64 *base = buf;
        if (size < sizeof(*base) || size < base->d_reclen) {
            return;
        }
        sb_bit_linux_dirent64(pr, base);
    } else {
        bit32_linux_dirent64 *base = buf;
        if (size < sizeof(*base) || size < base->d_reclen) {
            return;
        }
        sb_bit_linux_dirent64(pr, base);
    }
}

#ifndef sb_bit_linux_dirent
#define sb_bit_linux_dirent(pr_, s_) do { \
    sb_print_struct_top_member_hex_zero((pr_)->sb, s_, d_ino); \
    sb_print_struct_cnt_member_hex_zero((pr_)->sb, s_, d_off); \
    sb_print_struct_cnt_member_dec((pr_)->sb, s_, d_reclen); \
    string_buffer_strcat((pr_)->sb, ", .d_type = "); \
    string_buffer_hex_zero((pr_)->sb, (*(char*)((uintptr_t)s_ + s_->d_reclen - 1))); \
    string_buffer_strcat((pr_)->sb, ", .d_name = \""); \
    string_buffer_strcat((pr_)->sb, s_->d_name); \
} while (0)
#endif

void sb_print_array_prefix_linux_dirent(printer *pr) { sb_print_array_prefix_default(pr); }
void sb_print_array_suffix_linux_dirent(printer *pr) { sb_print_array_suffix_default(pr); }
void sb_print_elem_delim_linux_dirent(printer *pr) { sb_print_elem_delim_default(pr); }
size_t sb_print_elem_size_linux_dirent(printer *pr, void *buf, size_t size) {
    if (nrsi_is_bit64(pr->si)) {
        bit64_linux_dirent *base = buf;
        if (size < sizeof(*base) || size < base->d_reclen) {
            return size;
        }
        return base->d_reclen;
    } else {
        bit32_linux_dirent *base = buf;
        if (size < sizeof(*base) || size < base->d_reclen) {
            return size;
        }
        return base->d_reclen;
    }
}

void sb_print_linux_dirent(printer *pr, void *buf, size_t size)
{
    if (nrsi_is_bit64(pr->si)) {
        bit64_linux_dirent *base = buf;
        if (size < sizeof(*base) || size < base->d_reclen) {
            return;
        }
        sb_bit_linux_dirent(pr, base);
    } else {
        bit32_linux_dirent *base = buf;
        if (size < sizeof(*base) || size < base->d_reclen) {
            return;
        }
        sb_bit_linux_dirent(pr, base);
    }
}

