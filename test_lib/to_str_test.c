#include "../src/to_str.h"
#include "../src/to_str.c"
#include "../inc/util_header_all.h"
#include "test.h"


unsigned long long ullong_num[] = { 0, 1, 10, 100, 123, 102, LLONG_MIN >> 10, LLONG_MAX >> 8, LLONG_MIN, LLONG_MAX, ULLONG_MAX, (LLONG_MIN | LLONG_MIN >> 10) };
  signed long long sllong_num[] = { 0, 1, 10, 100, 123, 102, LLONG_MIN >> 10, LLONG_MAX >> 8, LLONG_MIN, LLONG_MAX, (long long)ULLONG_MAX, (LLONG_MIN | LLONG_MIN >> 10) };
unsigned long ulong_num[] = { 0, 1, 10, 100, 123, 102, LONG_MIN >> 10, LONG_MAX >> 8, LONG_MIN, LONG_MAX, ULONG_MAX, (LONG_MIN | LONG_MIN >> 10) };
  signed long slong_num[] = { 0, 1, 10, 100, 123, 102, LONG_MIN >> 10, LONG_MAX >> 8, LONG_MIN, LONG_MAX, (long)ULONG_MAX, (LONG_MIN | LONG_MIN >> 10) };
unsigned int uint_num[] = { 0, 1, 10, 100, 123, 102, INT_MIN >> 10, INT_MAX >> 10, INT_MIN, INT_MAX, UINT_MAX, (INT_MIN | INT_MIN >> 10) };
  signed int sint_num[] = { 0, 1, 10, 100, 123, 102, INT_MIN >> 10, INT_MAX >> 10, INT_MIN, INT_MAX, (int)UINT_MAX, (INT_MIN | INT_MIN >> 10) };
unsigned short ushort_num[] = { 0, 1, 10, 100, 123, 102, SHRT_MIN >> 10, SHRT_MAX >> 10, SHRT_MIN, SHRT_MAX, USHRT_MAX, (SHRT_MIN | SHRT_MIN >> 10) };
  signed short sshort_num[] = { 0, 1, 10, 100, 123, 102, SHRT_MIN >> 10, SHRT_MAX >> 10, SHRT_MIN, SHRT_MAX, (short)USHRT_MAX, (SHRT_MIN | SHRT_MIN >> 10) };
unsigned char uchar_num[] = { 0, 1, 10, 100, 123, 102, SCHAR_MIN >> (CHAR_BIT / 2), SCHAR_MAX >> (CHAR_BIT / 2), SCHAR_MIN, SCHAR_MAX, UCHAR_MAX };
  signed char schar_num[] = { 0, 1, 10, 100, 123, 102, SCHAR_MIN >> (CHAR_BIT / 2), SCHAR_MAX >> (CHAR_BIT / 2), SCHAR_MIN, SCHAR_MAX, UCHAR_MAX };
         char char_num[] = { 0, 1, 10, 100, 123, 102, SCHAR_MIN >> (CHAR_BIT / 2), SCHAR_MAX >> (CHAR_BIT / 2), SCHAR_MIN, SCHAR_MAX, UCHAR_MAX };

unsigned long long ullong_max = ULLONG_MAX;
  signed long long sllong_max = LLONG_MAX;
unsigned long ulong_max = ULONG_MAX;
  signed long slong_max = LONG_MAX;
unsigned int uint_max = UINT_MAX;
  signed int sint_max = INT_MAX;
unsigned short ushort_max = USHRT_MAX;
  signed short sshort_max = SHRT_MAX;
unsigned char uchar_max = UCHAR_MAX;
  signed char schar_max = SCHAR_MAX;
         char char_max = CHAR_MAX;

#define to_str_test(name_, h_, d_, o_) do { \
    for_each(e, name_##_num) { \
        char lv[32], rv[32], *r; \
        r = to_str_hex(lv, *e); \
        sprintf(rv, "0x%"h_, *e); \
        assert_str_eq(lv, rv); \
        assert_eq((size_t)(r - lv), strlen(lv)); \
        r = to_str_dec(lv, *e); \
        sprintf(rv, "%"d_, *e); \
        assert_str_eq(lv, rv); \
        assert_eq((size_t)(r - lv), strlen(lv)); \
        r = to_str_oct(lv, *e); \
        sprintf(rv, "0o%"o_, *e); \
        assert_str_eq(lv, rv); \
        assert_eq((size_t)(r - lv), strlen(lv)); \
    } \
} while (0)

#define to_width_str_test(name_, w_, h_, d_, o_) do { \
    for_each(e, name_##_num) { \
        char lv[32], rv[32], tmp[32], *r; \
        r = to_width_str_hex(lv, *e, w_); \
        sprintf(tmp, "0x%"h_, *e); \
        sprintf(rv, "%*s", w_, tmp); \
        assert_str_eq(lv, rv); \
        assert_eq((size_t)(r - lv), strlen(lv)); \
        r = to_width_str_dec(lv, *e, w_); \
        sprintf(tmp, "%"d_, *e); \
        sprintf(rv, "%*s", w_, tmp); \
        assert_str_eq(lv, rv); \
        assert_eq((size_t)(r - lv), strlen(lv)); \
        r = to_width_str_oct(lv, *e, w_); \
        sprintf(tmp, "0o%"o_, *e); \
        sprintf(rv, "%*s", w_, tmp); \
        assert_str_eq(lv, rv); \
        assert_eq((size_t)(r - lv), strlen(lv)); \
    } \
} while (0)

#define to_zero_str_test(name_, uname_, h_, d_, o_, u_) do { \
    for_each(e, name_##_num) { \
        char lv[32], rv[32], tmp[32], *r; \
        r = to_zero_str_hex(lv, *e); \
        sprintf(tmp, "%"h_"x", uname_##_max); \
        sprintf(rv, "0x%0*"h_"x", (int)strlen(tmp), *e); \
        assert_str_eq(lv, rv); \
        assert_eq((size_t)(r - lv), strlen(lv)); \
        r = to_zero_str_dec(lv, *e); \
        sprintf(tmp, "%"d_"u", uname_##_max); \
        if (u_) { sprintf(rv, "%0*"d_"u", (int)strlen(tmp), *e); } \
        else { sprintf(rv, "%0*"d_"d", (int)strlen(tmp) + (*e < 0), *e); } \
        assert_str_eq(lv, rv); \
        assert_eq((size_t)(r - lv), strlen(lv)); \
        r = to_zero_str_oct(lv, *e); \
        sprintf(tmp, "%"o_"o", uname_##_max); \
        sprintf(rv, "0o%0*"o_"o", (int)strlen(tmp), *e); \
        assert_str_eq(lv, rv); \
        assert_eq((size_t)(r - lv), strlen(lv)); \
    } \
} while (0)

void char_to_str_test () {
    if (CHAR_MIN == 0) { to_str_test(char, "hhx", "hhu", "hho"); }
    else { to_str_test(char, "hhx", "hhd", "hho"); }
}

void test () {
    LERR("char to str");
    if (CHAR_MIN == 0) { to_str_test(char, "hhx", "hhu", "hho"); }
    else { to_str_test(char, "hhx", "hhd", "hho"); }
    LERR("unsigned char to str");
    to_str_test(uchar, "hhx", "hhu", "hho");
    LERR("signed char to str");
    to_str_test(schar, "hhx", "hhd", "hho");
    LERR("unsigned short to str");
    to_str_test(ushort, "hx", "hu", "ho");
    LERR("signed short to str");
    to_str_test(sshort, "hx", "hd", "ho");
    LERR("unsigned int to str");
    to_str_test(uint, "x", "u", "o");
    LERR("signed int to str");
    to_str_test(sint, "x", "d", "o");
    LERR("unsigned long to str");
    to_str_test(ulong, "lx", "lu", "lo");
    LERR("signed long to str");
    to_str_test(slong, "lx", "ld", "lo");
    LERR("unsigned llong to str");
    to_str_test(ullong, "llx", "llu", "llo");
    LERR("signed llong to str");
    to_str_test(sllong, "llx", "lld", "llo");
}

void wtest () {
    LERR("char to str");
    if (CHAR_MIN == 0) { to_width_str_test(char, 5, "hhx", "hhu", "hho"); }
    else { to_width_str_test(char, 5, "hhx", "hhd", "hho"); }
    LERR("unsigned char to str");
    to_width_str_test(uchar, 5, "hhx", "hhu", "hho");
    LERR("signed char to str");
    to_width_str_test(schar, 10, "hhx", "hhd", "hho");
    LERR("unsigned short to str");
    to_width_str_test(ushort, 20, "hx", "hu", "ho");
    LERR("signed short to str");
    to_width_str_test(sshort, 30, "hx", "hd", "ho");
    LERR("unsigned int to str");
    to_width_str_test(uint, 30, "x", "u", "o");
    LERR("signed int to str");
    to_width_str_test(sint, 30, "x", "d", "o");
    LERR("unsigned long to str");
    to_width_str_test(ulong, 30, "lx", "lu", "lo");
    LERR("signed long to str");
    to_width_str_test(slong, 30, "lx", "ld", "lo");
    LERR("unsigned llong to str");
    to_width_str_test(ullong, 30, "llx", "llu", "llo");
    LERR("signed llong to str");
    to_width_str_test(sllong, 30, "llx", "lld", "llo");
}

void ztest () {
    LERR("char to str");
    if (CHAR_MIN == 0) { to_zero_str_test(char, uchar, "hh", "hh", "hh", 1); }
    else { to_zero_str_test(char, uchar, "hh", "hh", "hh", 0); }
    LERR("unsigned char to str");
    to_zero_str_test(uchar, uchar, "hh", "hh", "hh", 1);
    LERR("signed char to str");
    to_zero_str_test(schar, uchar, "hh", "hh", "hh", 0);
    LERR("unsigned short to str");
    to_zero_str_test(ushort, ushort, "h", "h", "h", 1);
    LERR("signed short to str");
    to_zero_str_test(sshort, ushort, "h", "h", "h", 0);
    LERR("unsigned int to str");
    to_zero_str_test(uint, uint, "", "", "", 1);
    LERR("signed int to str");
    to_zero_str_test(sint, uint, "", "", "", 0);
    LERR("unsigned long to str");
    to_zero_str_test(ulong, ulong, "l", "l", "l", 1);
    LERR("signed long to str");
    to_zero_str_test(slong, ulong, "l", "l", "l", 0);
    LERR("unsigned llong to str");
    to_zero_str_test(ullong, ullong, "ll", "ll", "ll", 1);
    LERR("signed llong to str");
    to_zero_str_test(sllong, ullong, "ll", "ll", "ll", 0);
}
int main () {
    test();
    wtest();
    ztest();
}
