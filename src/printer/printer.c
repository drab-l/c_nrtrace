#include <string.h>
#include <peek.h>
#include <table_uni.h>
#include <string_buffer.h>

#include "printer.h"
#include "syscall_info.h"

#include "stat.h"
#include "dirent.h"
#include "time.h"
#include "statfs.h"
#include "rlimit.h"
#include "poll.h"
#include "epoll.h"
#include "iovec.h"
#include "msghdr.h"

static void sb_print_strcat(printer *pr, bit_ptr_t addr)
{
    string_buffer *sb = pr->sb;
    pid_t pid = nrsi_pid(pr->si);
    static const size_t align = PEEK_DATA_UNIT_BYTE;
    bit_ptr_t addr_pad = addr % align;
    peek_t b;

    if (!addr) {
        return;
    }

    if (addr_pad) {
        char *s;
        addr -= addr_pad;
        if (peek_data(&b, sizeof(b), pid, addr)) {
            return;
        }
        s = addr_pad + (char *)&b;
        string_buffer_strcats(sb, s, align - addr_pad);
        if (memchr(s, 0, align - addr_pad)) {
            return;
        }
        addr += align;
    }

    while (!peek_data(&b, sizeof(b), pid, addr)) {
        string_buffer_strcats(sb, (const char*)&b, sizeof(b));
        if (memchr(&b, '\0', sizeof(b))) {
            return;
        }
        addr += align;
    }
}

static void sb_print_width_str(string_buffer *sb, const char *str, unsigned int width)
{
    size_t len = strlen(str);
    if (len < width) {
        string_buffer_charset(sb, ' ', width - len);
    }
    string_buffer_strcat(sb, str);
}

#define sb_print_number(pr_, num_, fmt_) (string_buffer_##fmt_(pr_->sb, num_))

#define sb_print_bit_number(pr_, num_, type_, fmt_) do { \
    if (nrsi_is_bit64(pr_->si)) { string_buffer_##fmt_(pr_->sb, (bit64_##type_)num_); } \
    else { string_buffer_##fmt_(pr_->sb, (bit32_##type_)num_); } \
} while (0)

#define peek_bit_number(pr_, addr_, type_) ( \
    nrsi_is_bit64(pr_->si) \
     ? peek_number((bit64_##type_)0, nrsi_pid(pr_->si), addr_) \
     : peek_number((bit32_##type_)0, nrsi_pid(pr_->si), addr_))

#define peek_sb_print_number(pr_, addr_, type_, fmt_) do { \
    if (!addr_) { string_buffer_strcat(pr->sb, "NULL"); } \
    else { \
        string_buffer_charcat(pr->sb, '{'); \
        string_buffer_##fmt_(pr_->sb, peek_number((type_)0, nrsi_pid(pr_->si), addr_)); \
        string_buffer_charcat(pr->sb, '}'); \
    } \
} while (0)

#define peek_sb_print_bit_number(pr_, addr_, type_, fmt_) do { \
    if (nrsi_is_bit64(pr_->si)) { peek_sb_print_number(pr_, addr_, bit64_##type_, fmt_); } \
    else { peek_sb_print_number(pr_, addr_, bit32_##type_, fmt_); } \
} while (0)

#define peek_sb_print_number_array(pr_, addr_, cnt_, type_, fmt_) do { \
    if (!addr_) { string_buffer_strcat(pr->sb, "NULL"); } \
    else { \
        string_buffer_charcat(pr->sb, '{'); \
        type_ PSPNA_buf[cnt_] = {}; \
        peek_data(&PSPNA_buf, sizeof(PSPNA_buf), nrsi_pid(pr_->si), addr_); \
        for_each(PSPNA_elem, PSPNA_buf) { \
            if (PSPNA_elem != PSPNA_buf) { \
                string_buffer_strcat(pr->sb, ", "); \
            }\
            string_buffer_##fmt_(pr_->sb, *PSPNA_elem); \
        } \
        string_buffer_charcat(pr->sb, '}'); \
    } \
} while (0)

#define peek_sb_print_bit_number_array(pr_, addr_, cnt_, type_, fmt_) do { \
    if (nrsi_is_bit64(pr_->si)) { peek_sb_print_number_array(pr_, addr_, cnt_, bit64_##type_, fmt_); } \
    else { peek_sb_print_number_array(pr_, addr_, cnt_, bit32_##type_, fmt_); } \
} while (0)


static void sb_print_graph_ascii_or_hex(printer *pr, void *buf, size_t size)
{
    unsigned char *b = buf;
    for (unsigned char *c = b; (ptrdiff_t)(c - b) < (ptrdiff_t)size; c++) {
        if (*c > 0x7f) {
            string_buffer_hexcat(pr->sb, buf, size);
            return;
        }
    }
    string_buffer_asciicat(pr->sb, buf, size);
}

static void peek_sb_print_graph_ascii_or_hex(printer *pr, bit_reg_t value, size_t len)
{
    unsigned char buf[4096];
    size_t max = sizeof(buf) < len ? sizeof(buf) : len;
    max -= peek_data(buf, max, nrsi_pid(pr->si), value);
    sb_print_graph_ascii_or_hex(pr, buf, max);
}

static void peek_sb_print_buffer_hex(printer *pr, bit_reg_t value, size_t len)
{
    unsigned char buf[4096];
    size_t max = sizeof(buf) < len ? sizeof(buf) : len;
    max -= peek_data(buf, max, nrsi_pid(pr->si), value);
    string_buffer_hexcat(pr->sb, buf, max);
}

static void peek_sb_print_str(printer *pr, bit_reg_t value)
{
    string_buffer_charcat(pr->sb, '"');
    sb_print_strcat(pr, value);
    string_buffer_charcat(pr->sb, '"');
}

static void peek_sb_print_str_array(printer *pr, bit_reg_t value)
{
    bit_ptr_t addr = value;
    bit_ptr_t array;
    size_t ptr_size = bit_sizeof(pr, ptr_t);
    string_buffer_charcat(pr->sb, '{');
    while ((array = peek_bit_number(pr, addr, ptr_t))) {
        peek_sb_print_str(pr, array);
        addr += ptr_size;
        string_buffer_strcat(pr->sb, ", ");
    }
    string_buffer_strcat(pr->sb, "NULL}");
}
static void peek_sb_print_length_str(printer *pr, bit_reg_t value, size_t len)
{
    char buf[4096];
    size_t max = sizeof(buf) < len ? sizeof(buf) : len;
    max -= peek_data(buf, max, nrsi_pid(pr->si), value);
    string_buffer_strcat(pr->sb, buf);
}

static void peek_sb_print_length_null_delim_str(printer *pr, bit_reg_t value, size_t len)
{
    char buf[4096];
    size_t max = sizeof(buf) < len ? sizeof(buf) : len;
    max -= peek_data(buf, max, nrsi_pid(pr->si), value);
    buf[sizeof(buf) - 1] = '\0';
    for (const char *s = buf; (size_t)(s - buf) < max; s += strlen(s) + 1) {
        if (s != buf) { string_buffer_strcat(pr->sb, ", "); }
        string_buffer_strcat(pr->sb, "\"");
        string_buffer_strcat(pr->sb, s);
        string_buffer_strcat(pr->sb, "\"");
    }
}

static void sb_print_enum(printer *pr, bit_reg_t value, const enum_table *table, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (table[i].num == value) {
            string_buffer_strcat(pr->sb, table[i].name);
            return;
        }
    }
    sb_print_number(pr, value, hex);
}

#define sb_print_enum(pr_, value_, cb_) do { \
    extern const enum_table enum_table_##cb_[enum_table_##cb_##_size]; \
    sb_print_enum(pr_, value_, enum_table_##cb_, enum_table_##cb_##_size); \
} while (0)

#define sb_print_callback(pr_, value_, cb_) do { \
    void sb_print_##cb_(printer*, bit_reg_t); \
    sb_print_##cb_(pr_, value_); \
} while (0)

typedef void sb_print_buf_func(printer*, void*, size_t);

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

#define peek_sb_print_array_callback_default(pr_, value_, len_, cb_, elemsize_) do { \
    void sb_print_##cb_(printer*, void*, size_t); \
    _Alignas(_Alignof(uint64_t)) unsigned char PSPAC_buf[4096]; \
    size_t PSPAC_max = sizeof(PSPAC_buf) < (size_t)(len_) ? sizeof(PSPAC_buf) : len_; \
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

#define declarate_sb_si(pr_) typeof(pr->sb) sb = pr->sb; typeof(pr->si) si = pr->si
static int print_any_type(printer *pr, bit_reg_t value, int print_type)
{
    void sb_print_stat(printer *pr, void *buf, size_t size);
    switch (print_type) {
    case SYSCALL_PRINT_INT : sb_print_number(pr, (sint)value, dec); break;
    case SYSCALL_PRINT_UINT : sb_print_number(pr, (uint)value, dec); break;
    case SYSCALL_PRINT_LONG : sb_print_number(pr, (slong)value, dec); break;
    case SYSCALL_PRINT_ULONG : sb_print_number(pr, (ulong)value, dec); break;
    case SYSCALL_PRINT_LLONG : sb_print_number(pr, (sllong)value, dec); break;
    case SYSCALL_PRINT_ULLONG : sb_print_number(pr, (ullong)value, dec); break;
    case SYSCALL_PRINT_ADDR : sb_print_bit_number(pr, value, ptr_t, hex_zero); break;
    case SYSCALL_PRINT_INT_HEX : sb_print_number(pr, (uint)value, hex); break;
    case SYSCALL_PRINT_REG_HEX : sb_print_bit_number(pr, value, reg_t, hex); break;
    case SYSCALL_PRINT_PTR2INT: peek_sb_print_bit_number(pr, value, sint, dec); break;
    case SYSCALL_PRINT_PTR2SIZE: peek_sb_print_bit_number(pr, value, size_t, hex_zero); break;
    case SYSCALL_PRINT_PTR2PTR: peek_sb_print_bit_number(pr, value, ptr_t, hex_zero); break;
    case SYSCALL_PRINT_PTR2VAR_3 : peek_sb_print_graph_ascii_or_hex(pr, value, nrsi_arg3(pr->si)); break;
    case SYSCALL_PRINT_PTR2VAR_4 : peek_sb_print_graph_ascii_or_hex(pr, value, nrsi_arg4(pr->si)); break;
    case SYSCALL_PRINT_PTR2VAR_R : peek_sb_print_graph_ascii_or_hex(pr, value, nrsi_argR(pr->si)); break;
    case SYSCALL_PRINT_PTR2HEX_3 : peek_sb_print_buffer_hex(pr, value, nrsi_arg3(pr->si)); break;
    case SYSCALL_PRINT_PTR2GETXATTR_VALUE :
        if (nrsi_argR(pr->si) > 0 && nrsi_arg4(pr->si) > 0) {
            peek_sb_print_graph_ascii_or_hex(pr, value, nrsi_argR(pr->si));
        } else {
            string_buffer_strcat(pr->sb, "{}");
        }
        break;
    case SYSCALL_PRINT_PTR2DIRENT_R : peek_sb_print_array_callback(pr, value, nrsi_argR(pr->si), linux_dirent); break;
    case SYSCALL_PRINT_PTR2DIRENT64_R : peek_sb_print_array_callback(pr, value, nrsi_argR(pr->si), linux_dirent64); break;
    case SYSCALL_PRINT_PTR2STAT_F : peek_sb_print_callback(pr, value, bit_sizeof(pr, stat), stat); break;
    case SYSCALL_PRINT_PTR2STAT64_F : peek_sb_print_callback(pr, value, bit_sizeof(pr, stat64), stat64); break;
    case SYSCALL_PRINT_PTR2STATX_F : peek_sb_print_callback(pr, value, sizeof(struct statx), statx); break;
    case SYSCALL_PRINT_PTR2STATFS_F : peek_sb_print_callback(pr, value, bit_sizeof(pr, statfs), statfs); break;
    case SYSCALL_PRINT_PTR2STATFS64_F : peek_sb_print_callback(pr, value, bit_sizeof(pr, statfs64), statfs64); break;
    case SYSCALL_PRINT_PTR2RLIMIT_F : peek_sb_print_callback(pr, value, bit_sizeof(pr, rlimit), rlimit); break;
    case SYSCALL_PRINT_PTR2RLIMIT64_F : peek_sb_print_callback(pr, value, sizeof(struct rlimit64), rlimit64); break;
    case SYSCALL_PRINT_PTR2TIMESPEC_F : peek_sb_print_callback(pr, value, bit_sizeof(pr, timespec), timespec); break;
    case SYSCALL_PRINT_PTR2EPOLL_EVENT_F : peek_sb_print_callback(pr, value, bit_sizeof(pr, epoll_event), epoll_event); break;
    case SYSCALL_PRINT_PTR2EPOLL_WAIT_EVENT_V : peek_sb_print_array_callback_default(pr, value, nrsi_argR(pr->si) * bit_sizeof(pr, epoll_event), epoll_event, bit_sizeof(pr, epoll_event)); break;
    case SYSCALL_PRINT_PTR2PPOLL_POLLFD_V : peek_sb_print_array_callback_default(pr, value, nrsi_argR(pr->si) * bit_sizeof(pr, pollfd), pollfd, bit_sizeof(pr, pollfd)); break;
    case SYSCALL_PRINT_PTR2SOCKADDR_3 : peek_sb_print_callback(pr, value, nrsi_arg3(pr->si), sockaddr); break;
    case SYSCALL_PRINT_PTR2SOCKADDR_6 : peek_sb_print_callback(pr, value, nrsi_arg6(pr->si), sockaddr); break;
    case SYSCALL_PRINT_PTR2SOCKADDR_PEEK3 : peek_sb_print_callback(pr, value, peek_bit_number(pr, nrsi_arg3(pr->si), socklen_t), sockaddr); break;
    case SYSCALL_PRINT_PTR2SOCKLEN: peek_sb_print_bit_number(pr, value, size_t, hex_zero); break;
    case SYSCALL_PRINT_PTR2STR_V: peek_sb_print_str(pr, value); break;
    case SYSCALL_PRINT_PTR2STR_R: peek_sb_print_length_str(pr, value, nrsi_argR(pr->si)); break;
    case SYSCALL_PRINT_PTR2STR_ARRAY: peek_sb_print_str_array(pr, value); break;
    case SYSCALL_PRINT_PTR2STR_NULL_DELIM_R: peek_sb_print_length_null_delim_str(pr, value, nrsi_argR(pr->si)); break;
    case SYSCALL_PRINT_PTR2PIPE_F: peek_sb_print_bit_number_array(pr, value, 2, sint, dec); break;
    case SYSCALL_PRINT_PTR2OFF_T_F: peek_sb_print_bit_number(pr, value, off_t, dec); break;
    case SYSCALL_PRINT_PTR2IOVEC_3 :
            pr->data = (void*)(uintptr_t)SIZE_MAX;
            peek_sb_print_array_callback_default(pr, value, nrsi_arg3(pr->si) * bit_sizeof(pr, iovec), iovec, bit_sizeof(pr, iovec));
            pr->data = NULL;
            break;
    case SYSCALL_PRINT_PTR2IOVEC_3_R :
            pr->data = (void*)(uintptr_t)nrsi_argR(pr->si);
            peek_sb_print_array_callback_default(pr, value, nrsi_arg3(pr->si) * bit_sizeof(pr, iovec), iovec, bit_sizeof(pr, iovec));
            pr->data = NULL;
            break;
    case SYSCALL_PRINT_PTR2MSGHDR : peek_sb_print_callback(pr, value, bit_sizeof(pr, msghdr), msghdr); break;
    case SYSCALL_PRINT_SOCKETCALL_NAME : sb_print_callback(pr, value, socketcall_name); break;
    case SYSCALL_PRINT_SOCKETCALL_ARG : sb_print_callback(pr, value, socketcall_arg); break;
    case SYSCALL_PRINT_CLOCK_ID : sb_print_enum(pr, value, clockid); break;
    default : sb_print_bit_number(pr, value, reg_t, hex); break;
    }
    return 0;
}

static enum syscall_print_type get_arg_print_type(printer *pr, int arg)
{
    int type = nrsi_print_type_argN(pr->si, arg);
    return type > SYSCALL_PRINT_LIMIT ? GET_SYSCALL_PRINT_IN(type) : type;
}

static enum syscall_print_type get_ret_arg_print_type(printer *pr, int arg)
{
    int type = nrsi_print_type_argN(pr->si, arg);
    return type > SYSCALL_PRINT_LIMIT ? GET_SYSCALL_PRINT_OUT(type) : SYSCALL_PRINT_NONE;
}

static enum syscall_print_type get_ret_print_type(printer *pr)
{
    return nrsi_print_type_argR(pr->si);
}

static void print_syscall_dump_reg(printer *pr)
{
    declarate_sb_si(pr);
    if (nrsi_is_exited(si)) {
        string_buffer_hex_zero(sb, nrsi_argR(si));
    } else {
        string_buffer_hex_zero(sb, nrsi_arg1(si));
        string_buffer_strcat(sb, ", ");
        string_buffer_hex_zero(sb, nrsi_arg2(si));
        string_buffer_strcat(sb, ", ");
        string_buffer_hex_zero(sb, nrsi_arg3(si));
        string_buffer_strcat(sb, ", ");
        string_buffer_hex_zero(sb, nrsi_arg4(si));
        string_buffer_strcat(sb, ", ");
        string_buffer_hex_zero(sb, nrsi_arg5(si));
        string_buffer_strcat(sb, ", ");
        string_buffer_hex_zero(sb, nrsi_arg6(si));
    }
}

static void print_syscall_header(printer *pr)
{
    declarate_sb_si(pr);
    string_buffer_strcat(sb, nrsi_is_exited(si) ? "out" : "in ");
    string_buffer_strcat(sb, " [");
    string_buffer_dec_width(sb, nrsi_pid(si), 6);
    string_buffer_strcat(sb, ":");
    string_buffer_dec_width(sb, nrsi_sys_num(si), 6);
    string_buffer_strcat(sb, "]");
    sb_print_width_str(sb, nrsi_sys_label(si), 20);
}

static void print_syscall_header_ommit_args(printer *pr)
{
    print_syscall_header(pr);
    string_buffer_strcat(pr->sb, "(...) = ");
}

static void print_syscall_out_args(printer *pr)
{
    int type = get_ret_arg_print_type(pr, 0);
    if (type != SYSCALL_PRINT_NONE) {
        string_buffer_strcat(pr->sb, ", 1:");
        print_any_type(pr, nrsi_arg1(pr->si), type);
    }
    type = get_ret_arg_print_type(pr, 1);
    if (type != SYSCALL_PRINT_NONE) {
        string_buffer_strcat(pr->sb, ", 2:");
        print_any_type(pr, nrsi_arg2(pr->si), type);
    }
    type = get_ret_arg_print_type(pr, 2);
    if (type != SYSCALL_PRINT_NONE) {
        string_buffer_strcat(pr->sb, ", 3:");
        print_any_type(pr, nrsi_arg3(pr->si), type);
    }
    type = get_ret_arg_print_type(pr, 3);
    if (type != SYSCALL_PRINT_NONE) {
        string_buffer_strcat(pr->sb, ", 4:");
        print_any_type(pr, nrsi_arg4(pr->si), type);
    }
    type = get_ret_arg_print_type(pr, 4);
    if (type != SYSCALL_PRINT_NONE) {
        string_buffer_strcat(pr->sb, ", 5:");
        print_any_type(pr, nrsi_arg5(pr->si), type);
    }
    type = get_ret_arg_print_type(pr, 5);
    if (type != SYSCALL_PRINT_NONE) {
        string_buffer_strcat(pr->sb, ", 6:");
        print_any_type(pr, nrsi_arg6(pr->si), type);
    }
}

static void print_syscall_ret(printer *pr)
{
    print_syscall_header_ommit_args(pr);
    bit_reg_t ret = nrsi_argR(pr->si);
    if (ret >= (bit_reg_t)-4096) {
        sb_print_number(pr, (int)ret, dec);
    } else {
        print_any_type(pr, ret, get_ret_print_type(pr));
        print_syscall_out_args(pr);
    }
    string_buffer_charcat(pr->sb, '\n');
}

static void print_syscall_args_impl(printer *pr)
{
    int type = get_arg_print_type(pr, 0);
    if (type == SYSCALL_PRINT_NONE) { return; }
    print_any_type(pr, nrsi_arg1(pr->si), type);

    type = get_arg_print_type(pr, 1);
    if (type == SYSCALL_PRINT_NONE) { return; }
    string_buffer_strcat(pr->sb, ", ");
    print_any_type(pr, nrsi_arg2(pr->si), type);

    type = get_arg_print_type(pr, 2);
    if (type == SYSCALL_PRINT_NONE) { return; }
    string_buffer_strcat(pr->sb, ", ");
    print_any_type(pr, nrsi_arg3(pr->si), type);

    type = get_arg_print_type(pr, 3);
    if (type == SYSCALL_PRINT_NONE) { return; }
    string_buffer_strcat(pr->sb, ", ");
    print_any_type(pr, nrsi_arg4(pr->si), type);

    type = get_arg_print_type(pr, 4);
    if (type == SYSCALL_PRINT_NONE) { return; }
    string_buffer_strcat(pr->sb, ", ");
    print_any_type(pr, nrsi_arg5(pr->si), type);

    type = get_arg_print_type(pr, 5);
    if (type == SYSCALL_PRINT_NONE) { return; }
    string_buffer_strcat(pr->sb, ", ");
    print_any_type(pr, nrsi_arg6(pr->si), type);
}

static void print_syscall_args(printer *pr)
{
    print_syscall_header(pr);
    string_buffer_charcat(pr->sb, '(');
    print_syscall_args_impl(pr);
    string_buffer_strcat(pr->sb, ")\n");
}

static void print_syscall_simple(printer *pr)
{
    print_syscall_header(pr);
    string_buffer_charcat(pr->sb, ' ');
    print_syscall_dump_reg(pr);
    string_buffer_charcat(pr->sb, '\n');
}

void printer_sb_print_syscall_out_args(printer *pr)
{
    print_syscall_out_args(pr);
}
void printer_sb_print_syscall_args(printer *pr)
{
    print_syscall_args_impl(pr);
}

void printer_peek_sb_print_graph_ascii_or_hex(printer *pr, bit_reg_t value, size_t len)
{
    peek_sb_print_graph_ascii_or_hex(pr, value, len);
}

int printer_print_syscall(string_buffer *sb, syscall_info_t *si)
{
    printer printer = {.sb = sb, .si = si,};
    bool print_skip = nrsi_is_print_skip(si);
    if (!print_skip) {
        if (!nrsi_is_print_configured(si)) {
            print_syscall_simple(&printer);
        } else {
            if (nrsi_is_exited(si)) {
                print_syscall_ret(&printer);
            } else {
                print_syscall_args(&printer);
            }
        }
    }
    return 0;
}
