#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include <util_header/type.h>

struct string_buffer;
typedef struct string_buffer string_buffer;
string_buffer *get_string_buffer(void);
void put_string_buffer(string_buffer *p);
void string_buffer_strcat(string_buffer *sb, const char *src);
void string_buffer_strcats(string_buffer *sb, const char *src, size_t size);
void string_buffer_hexcat(string_buffer *sb, const unsigned char *hex, size_t hex_len);
void string_buffer_asciicat(string_buffer *sb, const unsigned char *hex, size_t hex_len);
void string_buffer_charcat(string_buffer *sb, char c);
void string_buffer_fputs(string_buffer *sb, FILE *fp);
void string_buffer_set_empty(string_buffer *sb);
void string_buffer_charset(string_buffer *sb, const char c, size_t width);

#define string_buffer_hex_name(name_) string_buffer_##name_##_hex
#define string_buffer_dec_name(name_) string_buffer_##name_##_dec
#define string_buffer_oct_name(name_) string_buffer_##name_##_oct

#define string_buffer_hex_width_name(name_) string_buffer_##name_##_width_hex
#define string_buffer_dec_width_name(name_) string_buffer_##name_##_width_dec
#define string_buffer_oct_width_name(name_) string_buffer_##name_##_width_oct

#define string_buffer_hex_zero_name(name_) string_buffer_##name_##_zero_hex
#define string_buffer_dec_zero_name(name_) string_buffer_##name_##_zero_dec
#define string_buffer_oct_zero_name(name_) string_buffer_##name_##_zero_oct

#define declarate_string_buffer_us_type_number(type_) \
declarate_string_buffer_type_number(u##type_) \
declarate_string_buffer_type_number(s##type_) \

#define declarate_string_buffer_number(fmt_, type_) \
void string_buffer_##fmt_##_name(type_)(string_buffer *, type_); \
void string_buffer_##fmt_##_width_name(type_)(string_buffer *, type_, unsigned int); \
void string_buffer_##fmt_##_zero_name(type_)(string_buffer *, type_); \

#define declarate_string_buffer_type_number(type_) \
declarate_string_buffer_number(hex, type_) \
declarate_string_buffer_number(dec, type_) \
declarate_string_buffer_number(oct, type_) \

declarate_string_buffer_type_number(char)
declarate_string_buffer_us_type_number(char)
declarate_string_buffer_us_type_number(short)
declarate_string_buffer_us_type_number(int)
declarate_string_buffer_us_type_number(long)
declarate_string_buffer_us_type_number(llong)

#define string_buffer_generic_case(maker_, type_, ...) type_:maker_(type_)(__VA_ARGS__)
#define declarate_string_buffer_generic(maker_, type_, ...) _Generic((type_), \
string_buffer_generic_case(maker_, char __VA_OPT__(,) __VA_ARGS__), \
string_buffer_generic_case(maker_, uchar __VA_OPT__(,) __VA_ARGS__), string_buffer_generic_case(maker_, schar __VA_OPT__(,) __VA_ARGS__), \
string_buffer_generic_case(maker_, ushort __VA_OPT__(,) __VA_ARGS__), string_buffer_generic_case(maker_, sshort __VA_OPT__(,) __VA_ARGS__), \
string_buffer_generic_case(maker_, uint __VA_OPT__(,) __VA_ARGS__), string_buffer_generic_case(maker_, sint __VA_OPT__(,) __VA_ARGS__), \
string_buffer_generic_case(maker_, ulong __VA_OPT__(,) __VA_ARGS__), string_buffer_generic_case(maker_, slong __VA_OPT__(,) __VA_ARGS__), \
string_buffer_generic_case(maker_, ullong __VA_OPT__(,) __VA_ARGS__), string_buffer_generic_case(maker_, sllong __VA_OPT__(,) __VA_ARGS__))

#define string_buffer_hex(sb_, num_) declarate_string_buffer_generic(string_buffer_hex_name, num_, sb_, num_)
#define string_buffer_dec(sb_, num_) declarate_string_buffer_generic(string_buffer_dec_name, num_, sb_, num_)
#define string_buffer_oct(sb_, num_) declarate_string_buffer_generic(string_buffer_oct_name, num_, sb_, num_)

#define string_buffer_hex_width(sb_, num_, width_) declarate_string_buffer_generic(string_buffer_hex_width_name, num_, sb_, num_, width_)
#define string_buffer_dec_width(sb_, num_, width_) declarate_string_buffer_generic(string_buffer_dec_width_name, num_, sb_, num_, width_)
#define string_buffer_oct_width(sb_, num_, width_) declarate_string_buffer_generic(string_buffer_oct_width_name, num_, sb_, num_, width_)

#define string_buffer_hex_zero(sb_, num_) declarate_string_buffer_generic(string_buffer_hex_zero_name, num_, sb_, num_)
#define string_buffer_dec_zero(sb_, num_) declarate_string_buffer_generic(string_buffer_dec_zero_name, num_, sb_, num_)
#define string_buffer_oct_zero(sb_, num_) declarate_string_buffer_generic(string_buffer_oct_zero_name, num_, sb_, num_)

struct vl_array;
typedef struct vl_array vl_array;
vl_array *get_vl_array(size_t entry_size);
void *searched_or_tail_insert_vl_array_entry(vl_array *p, void *data, int(search)(const void*));
void *search_vl_array_entry(vl_array *p, int(search)(const void*, const void*), const void *arg);
void for_each_vl_array_entry(vl_array *p, void(*func)(void *,void*), void *arg);

