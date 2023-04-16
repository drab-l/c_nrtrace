#pragma once

#include "inc/printer.h"

#define bit_sizeof(pr_, type_) (nrsi_is_bit64(pr_->si) ? sizeof(bit64_##type_) : sizeof(bit32_##type_))

#define IMPL_SB_PRINT_BIT_TYPE(type_, pr_, buf_, size_) ( \
    (nrsi_is_bit64((pr_)->si)) \
        ? sb_print_bit64_##type_((pr_), ((bit64_##type_*)buf_), size_) \
        : sb_print_bit32_##type_((pr_), ((bit32_##type_*)buf_), size_) \
    )

#define IMPL_SB_PRINT_TYPE(type_, pr_, buf_, size_) do { \
    type_ *base = (buf_); \
    if ((size_) >= sizeof(*base)) { \
        sb_print_##type_((pr_), base, size_); \
    } \
} while (0)

#define sb_print_array_prefix_default(pr) string_buffer_strcat(pr->sb, "\n\t")
#define sb_print_array_suffix_default(pr) string_buffer_strcat(pr->sb, "\n\t")
#define sb_print_elem_delim_default(pr) string_buffer_strcat(pr->sb, ",\n\t")

#define sb_print_struct_top_member_hex(sb_, s_, member_) (string_buffer_strcat(sb_, "."#member_" = "), string_buffer_hex(sb_, s_->member_))
#define sb_print_struct_top_member_dec(sb_, s_, member_) (string_buffer_strcat(sb_, "."#member_" = "), string_buffer_dec(sb_, s_->member_))
#define sb_print_struct_top_member_oct(sb_, s_, member_) (string_buffer_strcat(sb_, "."#member_" = "), string_buffer_oct(sb_, s_->member_))

#define sb_print_struct_cnt_member_hex(sb_, s_, member_) (string_buffer_strcat(sb_, ", ."#member_" = "), string_buffer_hex(sb_, s_->member_))
#define sb_print_struct_cnt_member_dec(sb_, s_, member_) (string_buffer_strcat(sb_, ", ."#member_" = "), string_buffer_dec(sb_, s_->member_))
#define sb_print_struct_cnt_member_oct(sb_, s_, member_) (string_buffer_strcat(sb_, ", ."#member_" = "), string_buffer_oct(sb_, s_->member_))

#define sb_print_struct_top_member_hex_zero(sb_, s_, member_) (string_buffer_strcat(sb_, "."#member_" = "), string_buffer_hex_zero(sb_, s_->member_))
#define sb_print_struct_top_member_dec_zero(sb_, s_, member_) (string_buffer_strcat(sb_, "."#member_" = "), string_buffer_dec_zero(sb_, s_->member_))
#define sb_print_struct_top_member_oct_zero(sb_, s_, member_) (string_buffer_strcat(sb_, "."#member_" = "), string_buffer_oct_zero(sb_, s_->member_))

#define sb_print_struct_cnt_member_hex_zero(sb_, s_, member_) (string_buffer_strcat(sb_, ", ."#member_" = "), string_buffer_hex_zero(sb_, s_->member_))
#define sb_print_struct_cnt_member_dec_zero(sb_, s_, member_) (string_buffer_strcat(sb_, ", ."#member_" = "), string_buffer_dec_zero(sb_, s_->member_))
#define sb_print_struct_cnt_member_oct_zero(sb_, s_, member_) (string_buffer_strcat(sb_, ", ."#member_" = "), string_buffer_oct_zero(sb_, s_->member_))

void printer_peek_sb_print_graph_ascii_or_hex(printer *pr, bit_reg_t value, size_t len);
void printer_sb_print_syscall_out_args(printer *pr);
void printer_sb_print_syscall_args(printer *pr);

#define define_enum_table_elem(n_) {n_, #n_}
#define define_enum_table_elem_sentinel(n_) {0, NULL}
typedef struct { bit_reg_t num; const char *name; } enum_table;

#define peek_sb_print_callback(pr_, value_, len_, cb_) do { \
    _Alignas(_Alignof(uint64_t)) unsigned char PSPSC_buf[4096]; \
    size_t PSPSC_max = sizeof(PSPSC_buf) < (size_t)(len_) ? sizeof(PSPSC_buf) : len_; \
    PSPSC_max -= peek_data(PSPSC_buf, PSPSC_max, nrsi_pid(pr_->si), value_); \
    string_buffer_strcat(pr_->sb, "{"); \
    void sb_print_##cb_(printer*, void*, size_t); \
    sb_print_##cb_(pr_, PSPSC_buf, PSPSC_max); \
    string_buffer_strcat(pr_->sb, "}"); \
} while (0)

#define peek_sb_print_array_callback(pr_, value_, len_, cb_) do { \
    void sb_print_##cb_(printer*, void*, size_t); \
    void sb_print_array_prefix_##cb_(printer*); \
    void sb_print_array_suffix_##cb_(printer*); \
    void sb_print_elem_delim_##cb_(printer*); \
    size_t sb_print_elem_size_##cb_(printer*, void*, size_t); \
    _Alignas(_Alignof(uint64_t)) unsigned char PSPAC_buf[4096]; \
    size_t PSPAC_max = sizeof(PSPAC_buf) < (size_t)(len_) ? sizeof(PSPAC_buf) : len_; \
    PSPAC_max -= peek_data(PSPAC_buf, PSPAC_max, nrsi_pid(pr_->si), value_); \
    string_buffer_strcat(pr_->sb, "{"); \
    sb_print_array_prefix_##cb_(pr_); \
    size_t PSPAC_i = 0; \
    while (PSPAC_i < PSPAC_max) { \
        if (PSPAC_i) { sb_print_elem_delim_##cb_(pr_);} \
        string_buffer_strcat(pr_->sb, "{"); \
        sb_print_##cb_(pr_, &PSPAC_buf[PSPAC_i], PSPAC_max - PSPAC_i); \
        PSPAC_i += sb_print_elem_size_##cb_(pr_, &PSPAC_buf[PSPAC_i], PSPAC_max - PSPAC_i); \
        string_buffer_strcat(pr_->sb, "}"); \
    } \
    sb_print_array_suffix_##cb_(pr_); \
    string_buffer_strcat(pr_->sb, "}"); \
} while (0)

#define peek_sb_print_array_callback_default(pr_, value_, elemcnt_, cb_, elemsize_) do { \
    void sb_print_##cb_(printer*, void*, size_t); \
    _Alignas(_Alignof(uint64_t)) unsigned char PSPAC_buf[4096]; \
    size_t PSPAC_max = sizeof(PSPAC_buf) < (size_t)(elemcnt_ * elemsize_) ? sizeof(PSPAC_buf) : (elemcnt_ * elemsize_); \
    PSPAC_max -= peek_data(PSPAC_buf, PSPAC_max, nrsi_pid(pr_->si), value_); \
    string_buffer_strcat(pr_->sb, "{"); \
    sb_print_array_prefix_default(pr_); \
    size_t PSPAC_i = 0; \
    while (PSPAC_i < PSPAC_max) { \
        if (PSPAC_i) { sb_print_elem_delim_default(pr_);} \
        string_buffer_strcat(pr_->sb, "{"); \
        sb_print_##cb_(pr_, &PSPAC_buf[PSPAC_i], PSPAC_max - PSPAC_i); \
        PSPAC_i += elemsize_; \
        string_buffer_strcat(pr_->sb, "}"); \
    } \
    sb_print_array_suffix_default(pr_); \
    string_buffer_strcat(pr_->sb, "}"); \
} while (0)
