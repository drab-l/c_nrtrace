#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "inc/num_to_str.h"

#define FLOOR_NUMBER(n_,pre_, suf_) FLOOR_NUMBER_IN(n_,pre_, suf_)
#define FLOOR_NUMBER_IN(n_,pre_, suf_) FLOOR_NUMBER_##n_(pre_, suf_)
#define FLOOR_NUMBER_1(pre_, suf_) pre_##suf_
#define FLOOR_NUMBER_2(pre_, suf_) FLOOR_NUMBER_1(pre_##0, suf_)
#define FLOOR_NUMBER_3(pre_, suf_) FLOOR_NUMBER_2(pre_##0, suf_)
#define FLOOR_NUMBER_4(pre_, suf_) FLOOR_NUMBER_3(pre_##0, suf_)
#define FLOOR_NUMBER_5(pre_, suf_) FLOOR_NUMBER_4(pre_##0, suf_)
#define FLOOR_NUMBER_6(pre_, suf_) FLOOR_NUMBER_5(pre_##0, suf_)
#define FLOOR_NUMBER_7(pre_, suf_) FLOOR_NUMBER_6(pre_##0, suf_)
#define FLOOR_NUMBER_8(pre_, suf_) FLOOR_NUMBER_7(pre_##0, suf_)
#define FLOOR_NUMBER_9(pre_, suf_) FLOOR_NUMBER_8(pre_##0, suf_)
#define FLOOR_NUMBER_10(pre_, suf_) FLOOR_NUMBER_9(pre_##0, suf_)
#define FLOOR_NUMBER_11(pre_, suf_) FLOOR_NUMBER_10(pre_##0, suf_)
#define FLOOR_NUMBER_12(pre_, suf_) FLOOR_NUMBER_11(pre_##0, suf_)
#define FLOOR_NUMBER_13(pre_, suf_) FLOOR_NUMBER_12(pre_##0, suf_)
#define FLOOR_NUMBER_14(pre_, suf_) FLOOR_NUMBER_13(pre_##0, suf_)
#define FLOOR_NUMBER_15(pre_, suf_) FLOOR_NUMBER_14(pre_##0, suf_)
#define FLOOR_NUMBER_16(pre_, suf_) FLOOR_NUMBER_15(pre_##0, suf_)
#define FLOOR_NUMBER_17(pre_, suf_) FLOOR_NUMBER_16(pre_##0, suf_)
#define FLOOR_NUMBER_18(pre_, suf_) FLOOR_NUMBER_17(pre_##0, suf_)
#define FLOOR_NUMBER_19(pre_, suf_) FLOOR_NUMBER_18(pre_##0, suf_)
#define FLOOR_NUMBER_20(pre_, suf_) FLOOR_NUMBER_19(pre_##0, suf_)
#define FLOOR_NUMBER_21(pre_, suf_) FLOOR_NUMBER_20(pre_##0, suf_)
#define FLOOR_NUMBER_22(pre_, suf_) FLOOR_NUMBER_21(pre_##0, suf_)

#define INT8_HEX_FLOOR_MAX(suf_) FLOOR_NUMBER(INT8_HEX_DIG, 0x1, suf_)
#define INT8_DEC_FLOOR_MAX(suf_) FLOOR_NUMBER(INT8_DEC_DIG, 1, suf_)
#define INT8_OCT_FLOOR_MAX(suf_) FLOOR_NUMBER(INT8_OCT_DIG, 01, suf_)

#define INT16_HEX_FLOOR_MAX(suf_) FLOOR_NUMBER(INT16_HEX_DIG, 0x1, suf_)
#define INT16_DEC_FLOOR_MAX(suf_) FLOOR_NUMBER(INT16_DEC_DIG, 1, suf_)
#define INT16_OCT_FLOOR_MAX(suf_) FLOOR_NUMBER(INT16_OCT_DIG, 01, suf_)

#define INT32_HEX_FLOOR_MAX(suf_) FLOOR_NUMBER(INT32_HEX_DIG, 0x1, suf_)
#define INT32_DEC_FLOOR_MAX(suf_) FLOOR_NUMBER(INT32_DEC_DIG, 1, suf_)
#define INT32_OCT_FLOOR_MAX(suf_) FLOOR_NUMBER(INT32_OCT_DIG, 01, suf_)

#define INT64_HEX_FLOOR_MAX(suf_) FLOOR_NUMBER(INT64_HEX_DIG, 0x1, suf_)
#define INT64_DEC_FLOOR_MAX(suf_) FLOOR_NUMBER(INT64_DEC_DIG, 1, suf_)
#define INT64_OCT_FLOOR_MAX(suf_) FLOOR_NUMBER(INT64_OCT_DIG, 01, suf_)

#define EMPTY_MACRO
#if INT8_MAX == SCHAR_MAX
#define CHAR_HEX_FLOOR_MAX INT8_HEX_FLOOR_MAX(EMPTY_MACRO)
#define CHAR_DEC_FLOOR_MAX INT8_DEC_FLOOR_MAX(EMPTY_MACRO)
#define CHAR_OCT_FLOOR_MAX INT8_OCT_FLOOR_MAX(EMPTY_MACRO)
#else
#error invalit char width
#endif

#if INT8_MAX == SHRT_MAX
#error invalit short width
#elif INT16_MAX == SHRT_MAX
#define SHRT_HEX_FLOOR_MAX INT16_HEX_FLOOR_MAX(EMPTY_MACRO)
#define SHRT_DEC_FLOOR_MAX INT16_DEC_FLOOR_MAX(EMPTY_MACRO)
#define SHRT_OCT_FLOOR_MAX INT16_OCT_FLOOR_MAX(EMPTY_MACRO)
#elif INMAX == SHRT_MAX
#define SHRT_HEX_FLOOR_MAX INT32_HEX_FLOOR_MAX(EMPTY_MACRO)
#define SHRT_DEC_FLOOR_MAX INT32_DEC_FLOOR_MAX(EMPTY_MACRO)
#define SHRT_OCT_FLOOR_MAX INT32_OCT_FLOOR_MAX(EMPTY_MACRO)
#elif INT64_MAX == SHRT_MAX
#define SHRT_HEX_FLOOR_MAX INT64_HEX_FLOOR_MAX(EMPTY_MACRO)
#define SHRT_DEC_FLOOR_MAX INT64_DEC_FLOOR_MAX(EMPTY_MACRO)
#define SHRT_OCT_FLOOR_MAX INT64_OCT_FLOOR_MAX(EMPTY_MACRO)
#endif

#if INT8_MAX == INT_MAX
#error invalit int width
#elif INT16_MAX == INT_MAX
#define INT_HEX_FLOOR_MAX INT16_HEX_FLOOR_MAX(EMPTY_MACRO)
#define INT_DEC_FLOOR_MAX INT16_DEC_FLOOR_MAX(EMPTY_MACRO)
#define INT_OCT_FLOOR_MAX INT16_OCT_FLOOR_MAX(EMPTY_MACRO)
#elif INT32_MAX == INT_MAX
#define INT_HEX_FLOOR_MAX INT32_HEX_FLOOR_MAX(EMPTY_MACRO)
#define INT_DEC_FLOOR_MAX INT32_DEC_FLOOR_MAX(EMPTY_MACRO)
#define INT_OCT_FLOOR_MAX INT32_OCT_FLOOR_MAX(EMPTY_MACRO)
#elif INT64_MAX == INT_MAX
#define INT_HEX_FLOOR_MAX INT64_HEX_FLOOR_MAX(EMPTY_MACRO)
#define INT_DEC_FLOOR_MAX INT64_DEC_FLOOR_MAX(EMPTY_MACRO)
#define INT_OCT_FLOOR_MAX INT64_OCT_FLOOR_MAX(EMPTY_MACRO)
#endif

#if INT32_MAX == LONG_MAX
#define LONG_HEX_FLOOR_MAX INT32_HEX_FLOOR_MAX(LU)
#define LONG_DEC_FLOOR_MAX INT32_DEC_FLOOR_MAX(LU)
#define LONG_OCT_FLOOR_MAX INT32_OCT_FLOOR_MAX(LU)
#elif INT64_MAX == LONG_MAX
#define LONG_HEX_FLOOR_MAX INT64_HEX_FLOOR_MAX(LU)
#define LONG_DEC_FLOOR_MAX INT64_DEC_FLOOR_MAX(LU)
#define LONG_OCT_FLOOR_MAX INT64_OCT_FLOOR_MAX(LU)
#else
#error invalit long width
#endif

#if INT64_MAX == LLONG_MAX
#define LLONG_HEX_FLOOR_MAX INT64_HEX_FLOOR_MAX(LLU)
#define LLONG_DEC_FLOOR_MAX INT64_DEC_FLOOR_MAX(LLU)
#define LLONG_OCT_FLOOR_MAX INT64_OCT_FLOOR_MAX(LLU)
#else
#error invalit long width
#endif

#define FLOOR_MAX(x_, base_) _Generic(x_, \
char:CHAR_##base_##_FLOOR_MAX, schar: CHAR_##base_##_FLOOR_MAX, uchar: CHAR_##base_##_FLOOR_MAX, \
sshort:SHRT_##base_##_FLOOR_MAX, ushort: SHRT_##base_##_FLOOR_MAX, \
sint:INT_##base_##_FLOOR_MAX, uint: INT_##base_##_FLOOR_MAX, \
slong:LONG_##base_##_FLOOR_MAX, ulong: LONG_##base_##_FLOOR_MAX, \
sllong:LLONG_##base_##_FLOOR_MAX, ullong: LLONG_##base_##_FLOOR_MAX)

#define HEX_FLOOR_NUMBER(x_) FLOOR_NUMBER(x_, 0x1, LLU)
#define DEC_FLOOR_NUMBER(x_) FLOOR_NUMBER(x_, 1, LLU)
#define OCT_FLOOR_NUMBER(x_) FLOOR_NUMBER(x_, 01, LLU)

#define HEX_FLOOR_MAX(x_) FLOOR_MAX(x_, HEX)
#define DEC_FLOOR_MAX(x_) FLOOR_MAX(x_, DEC)
#define OCT_FLOOR_MAX(x_) FLOOR_MAX(x_, OCT)

static const char NUMBER_TABLE[16] = "0123456789abcdef";

#define to_str_proc(dst_, src_, floor_, base_) do { \
    if (floor_ <= src_) { \
        *dst_++ = NUMBER_TABLE[src_ / floor_]; \
        src_ %= floor_; \
    } else { \
        *dst_++ = '0'; \
    } \
    floor_ /= base_; \
} while (0)

#define define_type_to_str_in(type_) \
static char *type_##_to_str_in(char *dst, type_ src, typeof(src) floor, const uint base) \
{ \
    while (floor >= 1) { to_str_proc(dst, src, floor, base); } \
    *dst = '\0'; \
    return dst; \
}

#define generic_unsigned_name_call(type_, name_,...) _Generic((type_), \
uchar:uchar_##name_(__VA_ARGS__), ushort:ushort_##name_(__VA_ARGS__), \
uint:uint_##name_(__VA_ARGS__), ulong:ulong_##name_(__VA_ARGS__), \
ullong:ullong_##name_(__VA_ARGS__))

define_type_to_str_in(uchar)
define_type_to_str_in(ushort)
define_type_to_str_in(uint)
define_type_to_str_in(ulong)
define_type_to_str_in(ullong)
#define to_str_in(dst_, src_, floor_, base_) generic_unsigned_name_call(src_, to_str_in, dst, src_, floor_, base_)

#define unsigned_typeof(n_) typeof(_Generic(n_, char:(uchar)0, schar:(uchar)0, sshort:(ushort)0, sint:(uint)0, slong:(ulong)0, sllong:(ullong)0, default: n_))
#define is_unsigned_type(n_) _Generic(n_, char:!CHAR_MIN, uchar:1, ushort:1, uint:1, ulong:1, ullong:1, default: 0)

#define GET_DIG(src_, base_, base_name_, dig_, floor_) do { \
    if (src_ < base_) { dig_ = 1; floor_ = 0; break;} \
    dig_ = base_name_##_DIG_MAX(src_); \
    floor_ = base_name_##_FLOOR_MAX(src_); \
    while (floor_ > src_) { floor_ /= base_; dig_--; } \
} while (0)

#define prefix_proc(value_, dst_, base_) do { \
    if (value_ < 0 && base_ == 10) { *dst_++ = '-'; } \
    if (base_ == 0x10) { *dst_++ = '0'; *dst_++ = 'x'; } \
    else if (base_ == 010) { *dst_++ = '0'; *dst_++ = 'o'; } \
} while (0)

#define define_func_to_str(type_, base_name_low_, base_name_up_, base_) \
char *type_##_to_width_str_##base_name_low_(char *dst, type_ src, uint width) \
{ \
    if (src < 0 && base_ == 10) { width--; } \
    if (base_ == 0x10 || base_ == 010) { width -= 2; } \
    unsigned_typeof(src) usrc = (src >= 0 || base_ != 10) ? src : -src; \
    uchar dig; \
    unsigned_typeof(src) floor; \
    GET_DIG(usrc, base_, base_name_up_, dig, floor); \
    while (dig < width) { \
        *dst++ = ' '; \
        width--; \
    } \
    prefix_proc(src, dst, base_); \
    if (usrc < base_) { \
        *dst++ = NUMBER_TABLE[usrc]; \
        *dst = '\0'; \
        return dst; \
    } \
    return to_str_in(dst, usrc, floor, base_); \
} \
char *type_##_to_str_##base_name_low_(char *dst, type_ src) \
{ \
    prefix_proc(src, dst, base_); \
    unsigned_typeof(src) usrc = (src >= 0 || base_ != 10) ? src : -src; \
    if (usrc < base_) { \
        *dst++ = NUMBER_TABLE[usrc]; \
        *dst = '\0'; \
        return dst; \
    } \
    typeof(usrc) floor = base_name_up_##_FLOOR_MAX(usrc); \
    while (floor > usrc) { floor /= base_; } \
    return to_str_in(dst, usrc, floor, base_); \
} \
char *type_##_to_zero_str_##base_name_low_(char *dst, type_ src) \
{ \
    prefix_proc(src, dst, base_); \
    unsigned_typeof(src) usrc = (src >= 0 || base_ != 10) ? src : -src; \
    if (usrc < base_) { \
        memset(dst, '0', base_name_up_##_DIG_MAX(usrc)); \
        dst += base_name_up_##_DIG_MAX(usrc) - 1; \
        *dst++ = NUMBER_TABLE[usrc]; \
        *dst = '\0'; \
        return dst; \
    } \
    typeof(usrc) floor = base_name_up_##_FLOOR_MAX(usrc); \
    return to_str_in(dst, usrc, floor, base_); \
} \


#define define_funcs_to_str(type_) \
define_func_to_str(type_, hex, HEX, 0x10) \
define_func_to_str(type_, dec, DEC, 10) \
define_func_to_str(type_, oct, OCT, 010)

define_funcs_to_str(uchar)
define_funcs_to_str(ushort)
define_funcs_to_str(uint)
define_funcs_to_str(ulong)
define_funcs_to_str(ullong)

define_funcs_to_str(char)
define_funcs_to_str(schar)
define_funcs_to_str(sshort)
define_funcs_to_str(sint)
define_funcs_to_str(slong)
define_funcs_to_str(sllong)

