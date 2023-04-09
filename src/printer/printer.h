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

typedef struct { bit_reg_t num; const char *name; } enum_table;
