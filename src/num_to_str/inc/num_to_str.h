#pragma once

#include <stdint.h>
#include <limits.h>

#include <util_header/type.h>

#define INT8_HEX_DIG 2
#define INT8_DEC_DIG 3
#define INT8_OCT_DIG 3

#define INT16_HEX_DIG 4
#define INT16_DEC_DIG 5
#define INT16_OCT_DIG 6

#define INT32_HEX_DIG 8
#define INT32_DEC_DIG 10
#define INT32_OCT_DIG 11

#define INT64_HEX_DIG 16
#define INT64_DEC_DIG 20
#define INT64_OCT_DIG 22

#if INT8_MAX == SCHAR_MAX
#define CHAR_HEX_DIG INT8_HEX_DIG
#define CHAR_DEC_DIG INT8_DEC_DIG
#define CHAR_OCT_DIG INT8_OCT_DIG
#else
#error invalit char width
#endif

#if INT8_MAX == SHRT_MAX
#error invalit short width
#elif INT16_MAX == SHRT_MAX
#define SHRT_HEX_DIG INT16_HEX_DIG
#define SHRT_DEC_DIG INT16_DEC_DIG
#define SHRT_OCT_DIG INT16_OCT_DIG
#elif INMAX == SHRT_MAX
#define SHRT_HEX_DIG INT32_HEX_DIG
#define SHRT_DEC_DIG INT32_DEC_DIG
#define SHRT_OCT_DIG INT32_OCT_DIG
#elif INT64_MAX == SHRT_MAX
#define SHRT_HEX_DIG INT64_HEX_DIG
#define SHRT_DEC_DIG INT64_DEC_DIG
#define SHRT_OCT_DIG INT64_OCT_DIG
#endif

#if INT8_MAX == INT_MAX
#error invalit int width
#elif INT16_MAX == INT_MAX
#define INT_HEX_DIG INT16_HEX_DIG
#define INT_DEC_DIG INT16_DEC_DIG
#define INT_OCT_DIG INT16_OCT_DIG
#elif INT32_MAX == INT_MAX
#define INT_HEX_DIG INT32_HEX_DIG
#define INT_DEC_DIG INT32_DEC_DIG
#define INT_OCT_DIG INT32_OCT_DIG
#elif INT64_MAX == INT_MAX
#define INT_HEX_DIG INT64_HEX_DIG
#define INT_DEC_DIG INT64_DEC_DIG
#define INT_OCT_DIG INT64_OCT_DIG
#endif

#if INT32_MAX == LONG_MAX
#define LONG_HEX_DIG INT32_HEX_DIG
#define LONG_DEC_DIG INT32_DEC_DIG
#define LONG_OCT_DIG INT32_OCT_DIG
#elif INT64_MAX == LONG_MAX
#define LONG_HEX_DIG INT64_HEX_DIG
#define LONG_DEC_DIG INT64_DEC_DIG
#define LONG_OCT_DIG INT64_OCT_DIG
#else
#error invalit long width
#endif

#if INT64_MAX == LLONG_MAX
#define LLONG_HEX_DIG INT64_HEX_DIG
#define LLONG_DEC_DIG INT64_DEC_DIG
#define LLONG_OCT_DIG INT64_OCT_DIG
#else
#error invalit long width
#endif

#define DIG_MAX(x_, base_) _Generic(x_, \
char:CHAR_##base_##_DIG, schar: CHAR_##base_##_DIG, uchar: CHAR_##base_##_DIG, \
sshort:SHRT_##base_##_DIG, ushort: SHRT_##base_##_DIG, \
sint:INT_##base_##_DIG, uint: INT_##base_##_DIG, \
slong:LONG_##base_##_DIG, ulong: LONG_##base_##_DIG, \
sllong:LLONG_##base_##_DIG, ullong: LLONG_##base_##_DIG)

#define HEX_DIG_MAX(x_) DIG_MAX(x_, HEX)
#define DEC_DIG_MAX(x_) DIG_MAX(x_, DEC)
#define OCT_DIG_MAX(x_) DIG_MAX(x_, OCT)

#define declarate_func_to_str(name_prefix_, type_, base_name_low_) \
char *name_prefix_##_to_str_##base_name_low_(char *dst, type_ src); \
char *name_prefix_##_to_width_str_##base_name_low_(char *dst, type_ src, unsigned int width); \
char *name_prefix_##_to_zero_str_##base_name_low_(char *dst, type_ src);

#define declarate_funcs_to_str(name_prefix_, type_) \
declarate_func_to_str(name_prefix_, type_, hex) \
declarate_func_to_str(name_prefix_, type_, dec) \
declarate_func_to_str(name_prefix_, type_, oct)

declarate_funcs_to_str(uchar, unsigned char)
declarate_funcs_to_str(ushort, unsigned short)
declarate_funcs_to_str(uint, unsigned int)
declarate_funcs_to_str(ulong, unsigned long)
declarate_funcs_to_str(ullong, unsigned long long)

declarate_funcs_to_str(char,  char)
declarate_funcs_to_str(schar, signed char)
declarate_funcs_to_str(sshort, short)
declarate_funcs_to_str(sint, int)
declarate_funcs_to_str(slong, long)
declarate_funcs_to_str(sllong, long long)

#define to_str_hex(dst_, src_) _Generic((src_), \
char:char_to_str_hex(dst_, src_), unsigned char:uchar_to_str_hex(dst_, src_), unsigned short:ushort_to_str_hex(dst_, src_), \
unsigned int:uint_to_str_hex(dst_, src_), unsigned long:ulong_to_str_hex(dst_, src_), \
unsigned long long:ullong_to_str_hex(dst_, src_), signed char:schar_to_str_hex(dst_, src_), \
signed short:sshort_to_str_hex(dst_, src_), signed int:sint_to_str_hex(dst_, src_), \
signed long:slong_to_str_hex(dst_, src_), signed long long:sllong_to_str_hex(dst_, src_))

#define to_str_dec(dst_, src_) _Generic((src_), \
char:char_to_str_dec(dst_, src_), unsigned char:uchar_to_str_dec(dst_, src_), unsigned short:ushort_to_str_dec(dst_, src_), \
unsigned int:uint_to_str_dec(dst_, src_), unsigned long:ulong_to_str_dec(dst_, src_), \
unsigned long long:ullong_to_str_dec(dst_, src_), signed char:schar_to_str_dec(dst_, src_), \
signed short:sshort_to_str_dec(dst_, src_), signed int:sint_to_str_dec(dst_, src_), \
signed long:slong_to_str_dec(dst_, src_), signed long long:sllong_to_str_dec(dst_, src_))

#define to_str_oct(dst_, src_) _Generic((src_), \
char:char_to_str_oct(dst_, src_), unsigned char:uchar_to_str_oct(dst_, src_), unsigned short:ushort_to_str_oct(dst_, src_), \
unsigned int:uint_to_str_oct(dst_, src_), unsigned long:ulong_to_str_oct(dst_, src_), \
unsigned long long:ullong_to_str_oct(dst_, src_), signed char:schar_to_str_oct(dst_, src_), \
signed short:sshort_to_str_oct(dst_, src_), signed int:sint_to_str_oct(dst_, src_), \
signed long:slong_to_str_oct(dst_, src_), signed long long:sllong_to_str_oct(dst_, src_))

#define to_width_str_hex(dst_, src_, width_) _Generic((src_), \
char:char_to_width_str_hex(dst_, src_, width_), unsigned char:uchar_to_width_str_hex(dst_, src_, width_), unsigned short:ushort_to_width_str_hex(dst_, src_, width_), \
unsigned int:uint_to_width_str_hex(dst_, src_, width_), unsigned long:ulong_to_width_str_hex(dst_, src_, width_), \
unsigned long long:ullong_to_width_str_hex(dst_, src_, width_), signed char:schar_to_width_str_hex(dst_, src_, width_), \
signed short:sshort_to_width_str_hex(dst_, src_, width_), signed int:sint_to_width_str_hex(dst_, src_, width_), \
signed long:slong_to_width_str_hex(dst_, src_, width_), signed long long:sllong_to_width_str_hex(dst_, src_, width_))

#define to_width_str_dec(dst_, src_, width_) _Generic((src_), \
char:char_to_width_str_dec(dst_, src_, width_), unsigned char:uchar_to_width_str_dec(dst_, src_, width_), unsigned short:ushort_to_width_str_dec(dst_, src_, width_), \
unsigned int:uint_to_width_str_dec(dst_, src_, width_), unsigned long:ulong_to_width_str_dec(dst_, src_, width_), \
unsigned long long:ullong_to_width_str_dec(dst_, src_, width_), signed char:schar_to_width_str_dec(dst_, src_, width_), \
signed short:sshort_to_width_str_dec(dst_, src_, width_), signed int:sint_to_width_str_dec(dst_, src_, width_), \
signed long:slong_to_width_str_dec(dst_, src_, width_), signed long long:sllong_to_width_str_dec(dst_, src_, width_))

#define to_width_str_oct(dst_, src_, width_) _Generic((src_), \
char:char_to_width_str_oct(dst_, src_, width_), unsigned char:uchar_to_width_str_oct(dst_, src_, width_), unsigned short:ushort_to_width_str_oct(dst_, src_, width_), \
unsigned int:uint_to_width_str_oct(dst_, src_, width_), unsigned long:ulong_to_width_str_oct(dst_, src_, width_), \
unsigned long long:ullong_to_width_str_oct(dst_, src_, width_), signed char:schar_to_width_str_oct(dst_, src_, width_), \
signed short:sshort_to_width_str_oct(dst_, src_, width_), signed int:sint_to_width_str_oct(dst_, src_, width_), \
signed long:slong_to_width_str_oct(dst_, src_, width_), signed long long:sllong_to_width_str_oct(dst_, src_, width_))

#define to_zero_str_hex(dst_, src_) _Generic((src_), \
char:char_to_zero_str_hex(dst_, src_), unsigned char:uchar_to_zero_str_hex(dst_, src_), unsigned short:ushort_to_zero_str_hex(dst_, src_), \
unsigned int:uint_to_zero_str_hex(dst_, src_), unsigned long:ulong_to_zero_str_hex(dst_, src_), \
unsigned long long:ullong_to_zero_str_hex(dst_, src_), signed char:schar_to_zero_str_hex(dst_, src_), \
signed short:sshort_to_zero_str_hex(dst_, src_), signed int:sint_to_zero_str_hex(dst_, src_), \
signed long:slong_to_zero_str_hex(dst_, src_), signed long long:sllong_to_zero_str_hex(dst_, src_))

#define to_zero_str_dec(dst_, src_) _Generic((src_), \
char:char_to_zero_str_dec(dst_, src_), unsigned char:uchar_to_zero_str_dec(dst_, src_), unsigned short:ushort_to_zero_str_dec(dst_, src_), \
unsigned int:uint_to_zero_str_dec(dst_, src_), unsigned long:ulong_to_zero_str_dec(dst_, src_), \
unsigned long long:ullong_to_zero_str_dec(dst_, src_), signed char:schar_to_zero_str_dec(dst_, src_), \
signed short:sshort_to_zero_str_dec(dst_, src_), signed int:sint_to_zero_str_dec(dst_, src_), \
signed long:slong_to_zero_str_dec(dst_, src_), signed long long:sllong_to_zero_str_dec(dst_, src_))

#define to_zero_str_oct(dst_, src_) _Generic((src_), \
char:char_to_zero_str_oct(dst_, src_), unsigned char:uchar_to_zero_str_oct(dst_, src_), unsigned short:ushort_to_zero_str_oct(dst_, src_), \
unsigned int:uint_to_zero_str_oct(dst_, src_), unsigned long:ulong_to_zero_str_oct(dst_, src_), \
unsigned long long:ullong_to_zero_str_oct(dst_, src_), signed char:schar_to_zero_str_oct(dst_, src_), \
signed short:sshort_to_zero_str_oct(dst_, src_), signed int:sint_to_zero_str_oct(dst_, src_), \
signed long:slong_to_zero_str_oct(dst_, src_), signed long long:sllong_to_zero_str_oct(dst_, src_))
