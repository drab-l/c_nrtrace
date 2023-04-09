#include <vl_buffer_lib.h>
#include <num_to_str.h>
#include "inc/string_buffer.h"

/* vlb string buffer */
struct string_buffer {vlb vl; struct {char *b;} type[];};

string_buffer *get_string_buffer(void)
{
    vlb *r;
    vlb_alloc(&r);
    if (r) {
        string_buffer *sb = (string_buffer*)r;
        *GET_TAIL_PTR(sb) = '\0';
        ADD_USED_SIZE(sb, 1);
    }
    return (string_buffer*)r;
}

static char *get_last_null_ptr(string_buffer *sb)
{
    return GET_TAIL_PTR(sb) - 1;
}

void put_string_buffer(string_buffer *p)
{
    vlb_dealloc((vlb*)p);
}

void string_buffer_set_empty(string_buffer *sb)
{
    vlb_clear_used_size((vlb*)sb);
    *GET_TAIL_PTR(sb) = '\0';
    ADD_USED_SIZE(sb, 1);
}

void string_buffer_charcat(string_buffer *sb, char c)
{
    EXPAND_BUFFER_IF_NEED(sb, 1);
    char *s = get_last_null_ptr(sb);
    s[0] = c; s[1] = '\0';
    ADD_USED_SIZE(sb, 1);
}

void string_buffer_hexcat(string_buffer *sb, const unsigned char *hex, size_t hex_len)
{
    EXPAND_BUFFER_IF_NEED(sb, hex_len * 2);
    static const char hex_table[16] = "0123456789abcdef";
    char *buf = get_last_null_ptr(sb);
    size_t total = 0;
    while (hex_len--) {
        buf[total++] = hex_table[(hex[0] >> 4) & 0xF];
        buf[total++] = hex_table[hex[0] & 0xF];
        hex++;
    }
    buf[total] = '\0';
    ADD_USED_SIZE(sb, total);
}

void string_buffer_asciicat(string_buffer *sb, const unsigned char *hex, size_t hex_len)
{
    size_t total = 0;
    // calculate length.
    for (size_t i = 0; i < hex_len; i++) {
        total += (hex[i] >= 0x20 && hex[i] <= 0x7E) ? 1 : 4;
    }
    EXPAND_BUFFER_IF_NEED(sb, total + 2);

    static const char hex_table[16] = "0123456789abcdef";
    char *s = get_last_null_ptr(sb);
    total = 0;
    s[total++] = '"';
    for (size_t i = 0; i < hex_len; i++) {
        if (hex[i] >= 0x20 && hex[i] <= 0x7E) {
            // printable character
            s[total++] = hex[i];
        } else {
            // not printable character, dump hex
            s[total++] = '\\';
            s[total++] = 'x';
            s[total++] = hex_table[(hex[i] >> 4) & 0xF];
            s[total++] = hex_table[hex[i] & 0xF];
        }
    }
    s[total++] = '"';
    s[total] = '\0';
    ADD_USED_SIZE(sb, total);
}

void string_buffer_strcat(string_buffer *sb, const char *src)
{
    if (src) {
        size_t size = strlen(src);
        EXPAND_BUFFER_IF_NEED(sb, size);
        strcpy(get_last_null_ptr(sb), src);
        ADD_USED_SIZE(sb, size);
    }
}

void string_buffer_strcats(string_buffer *sb, const char *src, size_t size)
{
    if (src) {
        size = strnlen(src, size);
        EXPAND_BUFFER_IF_NEED(sb, size);
        memcpy(get_last_null_ptr(sb), src, size);
        get_last_null_ptr(sb)[size] = '\0';
        ADD_USED_SIZE(sb, size);
    }
}

void string_buffer_charset(string_buffer *sb, char c, size_t width)
{
    EXPAND_BUFFER_IF_NEED(sb, width);
    memset(get_last_null_ptr(sb), c, width);
    get_last_null_ptr(sb)[width] = '\0';
    ADD_USED_SIZE(sb, width);
}

void string_buffer_fputs(string_buffer *sb, FILE *fp)
{
    fputs(GET_HEAD_PTR(sb), fp);
    fflush(fp);
}

#define define_string_buffer_type_number_func(type_, blow_, bup_) \
void string_buffer_##blow_##_name(type_)(string_buffer *sb, type_ num) { \
    unsigned int dig = 3 + bup_##_DIG_MAX(num); \
    EXPAND_BUFFER_IF_NEED(sb, dig); \
    char *s = get_last_null_ptr(sb); \
    char *e = to_str_##blow_(s, num); \
    ADD_USED_SIZE(sb, pointer_diff(e, s)); \
}
#define define_string_buffer_type_width_number_func(type_, blow_, bup_) \
void string_buffer_##blow_##_width_name(type_)(string_buffer *sb, type_ num, unsigned int width) { \
    unsigned int dig = 3 + bup_##_DIG_MAX(num); \
    EXPAND_BUFFER_IF_NEED(sb, dig); \
    char *s = get_last_null_ptr(sb); \
    char *e = to_width_str_##blow_(s, num, width); \
    ADD_USED_SIZE(sb, pointer_diff(e, s)); \
}
#define define_string_buffer_type_zero_number_func(type_, blow_, bup_) \
void string_buffer_##blow_##_zero_name(type_)(string_buffer *sb, type_ num) { \
    unsigned int dig = 3 + bup_##_DIG_MAX(num); \
    EXPAND_BUFFER_IF_NEED(sb, dig); \
    char *s = get_last_null_ptr(sb); \
    char *e = to_zero_str_##blow_(s, num); \
    ADD_USED_SIZE(sb, pointer_diff(e, s)); \
}

#define define_string_buffer_type_number_funcs(type_) \
define_string_buffer_type_number_func(type_, hex, HEX) \
define_string_buffer_type_number_func(type_, dec, DEC) \
define_string_buffer_type_number_func(type_, oct, OCT) \

#define define_string_buffer_type_witdh_number_funcs(type_) \
define_string_buffer_type_width_number_func(type_, hex, HEX) \
define_string_buffer_type_width_number_func(type_, dec, DEC) \
define_string_buffer_type_width_number_func(type_, oct, OCT) \

#define define_string_buffer_type_zero_number_funcs(type_) \
define_string_buffer_type_zero_number_func(type_, hex, HEX) \
define_string_buffer_type_zero_number_func(type_, dec, DEC) \
define_string_buffer_type_zero_number_func(type_, oct, OCT) \

#define define_string_buffer_type_number(type_) \
define_string_buffer_type_number_funcs(type_) \
define_string_buffer_type_witdh_number_funcs(type_) \
define_string_buffer_type_zero_number_funcs(type_) \

#define define_string_buffer_us_type_number(type_) \
define_string_buffer_type_number(u##type_) \
define_string_buffer_type_number(s##type_)

define_string_buffer_type_number(char)
define_string_buffer_us_type_number(char)
define_string_buffer_us_type_number(short)
define_string_buffer_us_type_number(int)
define_string_buffer_us_type_number(long)
define_string_buffer_us_type_number(llong)

