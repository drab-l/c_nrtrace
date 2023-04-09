#pragma once
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

#define DEF_DEFAULT_SB_PRINT(bit_, name_) static void sb_bit##bit_##_##name_(string_buffer *sb, const bit##bit_##_##name_ *s) { sb_bit_##name_(sb, s); }

#define CALL_SB_DUMP(sb_, ptr_, si_, name_) (nrsi_is_bit64((si_)) ? sb_bit64_##name_(sb_, ptr_) : sb_bit32_##name_(sb_, ptr_))

