#include <string.h>
#define LERR(l_) fputs(l_"\n", stdout)
#define LEPRINT(fmt_, ...) fprintf(stdout, fmt_"\n" __VA_OPT__(,) __VA_ARGS__)

#define assert_eq(x_, y_) do { \
    typeof(x_) AE_lv_= x_; typeof (y_) AE_rv_ = y_; \
    if (AE_lv_ == AE_rv_) { \
        LERR("OK:left: "#x_ ", right: "#y_); \
    }  else { \
        LERR("NG:left: "#x_ ", right: "#y_); \
    }\
} while (0)

#define assert_str_eq(x_, y_) do { \
    const char *AE_lv_= x_, *AE_rv_ = y_; \
    if (!strcmp(AE_lv_, AE_rv_)) { \
        LEPRINT("OK: left: %s, right: %s", AE_lv_, AE_rv_); \
    } else {\
        LEPRINT("NG: left: %s,\n" \
                "   right: %s,", AE_lv_, AE_rv_); \
    } \
} while (0)
