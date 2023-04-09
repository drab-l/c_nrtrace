#pragma once

#if __STDC_VERSION__ <= 201710L
#define assert_static_(x_) _Static_assert(x_, #x_)
#else
#define assert_static_(x_) _Static_assert(x_)
#endif

#ifndef __has_c_attribute
#define __has_c_attribute(x) 0
#endif

#if __has_c_attribute(maybe_unused)
#define unused_ [[maybe_unused]]
#else
#define unused_ __attribute__((unused))
#endif

#if __has_c_attribute(noreturn)
#define noreturn_ [[noreturn]]
#else
#define noreturn_ __attribute__((noreturn))
#endif
