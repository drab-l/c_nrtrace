#include <linux/net.h>
#include <util_header_all.h>
#include <peek.h>

#include "sockcall.h"
#include "syscall_info.h"

static const struct {const char *name; unsigned long num;} socketcall_table[] = {
    [SYS_SOCKET] = {.name = "socket", .num = sys_uni_socket,},
    [SYS_BIND] = {.name = "bind", .num = sys_uni_bind,},
    [SYS_CONNECT] = {.name = "connect", .num = sys_uni_connect,},
    [SYS_LISTEN] = {.name = "listen", .num = sys_uni_listen,},
    [SYS_ACCEPT] = {.name = "accept", .num = sys_uni_accept,},
    [SYS_GETSOCKNAME] = {.name = "getsockname", .num = sys_uni_getsockname,},
    [SYS_GETPEERNAME] = {.name = "getpeername", .num = sys_uni_getpeername,},
    [SYS_SOCKETPAIR] = {.name = "socketpair", .num = sys_uni_socketpair,},
    [SYS_SEND] = {.name = "send", .num = sys_uni_send,},
    [SYS_RECV] = {.name = "recv", .num = sys_uni_recv,},
    [SYS_SENDTO] = {.name = "sendto", .num = sys_uni_sendto,},
    [SYS_RECVFROM] = {.name = "recvfrom", .num = sys_uni_recvfrom,},
    [SYS_SHUTDOWN] = {.name = "shutdown", .num = sys_uni_shutdown,},
    [SYS_SETSOCKOPT] = {.name = "setsockopt", .num = sys_uni_setsockopt,},
    [SYS_GETSOCKOPT] = {.name = "getsockopt", .num = sys_uni_getsockopt,},
    [SYS_SENDMSG] = {.name = "sendmsg", .num = sys_uni_sendmsg,},
    [SYS_RECVMSG] = {.name = "recvmsg", .num = sys_uni_recvmsg,},
    [SYS_ACCEPT4] = {.name = "accept4", .num = sys_uni_accept4,},
    [SYS_RECVMMSG] = {.name = "recvmmsg", .num = sys_uni_recvmmsg,},
    [SYS_SENDMMSG] = {.name = "sendmmsg", .num = sys_uni_sendmmsg,},
};

void sb_print_socketcall_name(printer *pr, bit_reg_t value)
{
    string_buffer_strcat(pr->sb, value < array_count(socketcall_table) ? socketcall_table[value].name : "unknown");
}

void sb_print_socketcall_arg(printer *pr, bit_reg_t value)
{
    static unsigned char n[] = {0, 3, 3, 3, 2, 3, 3, 3, 4, 4, 4, 6, 6, 2, 5, 5, 3, 3, 4, 5, 4};
    bit_reg_t s = nrsi_arg1(pr->si); // check socketcall number
    if (s >= array_count(n)) {
        string_buffer_hex(pr->sb, s);
        return;
    }
    string_buffer_charcat(pr->sb, '{');
    // make dummy data for dump arguments
    syscall_info_buf buf;
    syscall_info_t *p = (void*)&buf;
    nrsi_copy(p, pr->si);
    bit_reg_t b[6] = {};
    if (nrsi_is_bit64(pr->si)) {
        peek_data(b, n[s] * sizeof(b[0]), nrsi_pid(pr->si), value);
    } else {
        bit32_reg_t b32[6] = {};
        peek_data(b32, n[s] * sizeof(b32[0]), nrsi_pid(pr->si), value);
        for (size_t i = 0; i < n[s]; i++) {
            b[i] = b32[i];
        }
    }
    nrsi_set_sys_num(p, s);
    nrsi_set_args(p, b);
    nrsi_set_uni_sys_num(p, socketcall_table[s].num);
    if (nrsi_is_exited(p)) {
        sb_print_socketcall_name(pr, s);
        printer_sb_print_syscall_out_args(&(printer){.sb = pr->sb, .si = p});
    } else {
        printer_sb_print_syscall_args(&(printer){.sb = pr->sb, .si = p});
    }
    for (size_t i = 0; i < n[s]; i++) {
    }
    string_buffer_charcat(pr->sb, '}');

}
