#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <linux/ptrace.h>

#include <type_uni.h>
#include <util_header_all.h>
#include "inc/syscall_info.h"
#include "inc/peek.h"

static int peek_data_once(peek_t *buf, pid_t pid, bit_ptr_t addr)
{
    errno = 0;
    *buf = ptrace(PTRACE_PEEKDATA, pid, (long*)addr, NULL);
    if (*buf == -1 && errno) {
        return -1;
    }
    return 0;
}

static void memcpy_skip(void *dst, void *src, size_t size, size_t skip)
{
    unsigned char *s = src;
    s += skip;
    memcpy(dst, s, size);
}

size_t peek_data(void *buf, size_t size, pid_t pid, bit_ptr_t addr)
{
    static const size_t align = PEEK_DATA_UNIT_BYTE;
    ptrdiff_t addr_pad = addr % align;
    unsigned char *dst = buf;
    peek_t b;

    if (!size || !addr) {
        return size;
    }

    if (addr_pad) {
        if (peek_data_once(&b, pid, addr - addr_pad)) {
            return size;
        }
        size_t cpy = size < align - addr_pad ? size : align - addr_pad;
        memcpy_skip(dst, &b, cpy, addr_pad);
        addr += cpy;
        size -= cpy;
        dst += cpy;
    }

    while (size && !peek_data_once(&b, pid, addr)) {
        size_t cpy = size <= align ? size : align;
        memcpy(dst, &b, cpy);
        size -= cpy;
        dst += cpy;
        addr += align;
    }
    return size;
}

#define define_peek_number(type_) \
type_ peek_number_##type_(pid_t pid, bit_ptr_t addr) \
{ \
    type_ dst; \
    peek_data(&dst, sizeof(dst), pid, addr); \
    return dst; \
}

define_peek_number(char)
define_peek_number(schar)
define_peek_number(uchar)
define_peek_number(sshort)
define_peek_number(ushort)
define_peek_number(sint)
define_peek_number(uint)
define_peek_number(slong)
define_peek_number(ulong)
define_peek_number(sllong)
define_peek_number(ullong)

static pid_t peek_get_new_proc_pid(pid_t parent)
{
    unsigned long new;
    ptrace(PTRACE_GETEVENTMSG, parent, NULL, &new);
    return new;
}

static pid_t attach_running_proc(pid_t pid)
{
    static const int opt = PTRACE_O_TRACEEXEC | PTRACE_O_TRACEFORK | PTRACE_O_TRACEVFORK | PTRACE_O_TRACECLONE | PTRACE_O_TRACESYSGOOD;
    ptrace(PTRACE_SEIZE, pid, NULL, (void*)(uintptr_t)opt);
    return pid;
}

static void cont_listen_proc(pid_t pid)
{
    ptrace(PTRACE_LISTEN, pid, NULL, NULL);
}

static void cont_sys_proc(pid_t pid, int sig)
{
    ptrace(PTRACE_SYSCALL, pid, NULL, (void*)(uintptr_t)sig);
}

static int is_fork_stop(int status)
{
    if (!WIFSTOPPED(status)) {
        return 0;
    }
    static const int fork_stop = SIGTRAP | (PTRACE_EVENT_FORK << 8);
    static const int vfork_stop = SIGTRAP | (PTRACE_EVENT_VFORK << 8);
    static const int clone_stop = SIGTRAP | (PTRACE_EVENT_CLONE << 8);
    status >>= 8;
    return (status == fork_stop || status == vfork_stop || status == clone_stop);
}

static int is_event_stop(int status)
{
    return (status >> 16) == PTRACE_EVENT_STOP;
}

static int is_group_stop_sig(int status)
{
    const int sig = WSTOPSIG(status);
    return sig == SIGSTOP || sig == SIGTSTP  || sig == SIGTTIN || sig == SIGTTOU;
}

static int is_group_stop(int status)
{
    return is_event_stop(status) && is_group_stop_sig(status);
}

static int is_syscall_stop(int status)
{
    static const int stop = SIGTRAP | 0x80;
    return WIFSTOPPED(status) && WSTOPSIG(status) == stop;
}

static int is_exec_stop(int status)
{
    if (!WIFSTOPPED(status)) {
        return 0;
    }
    static const int exec_stop = SIGTRAP | (PTRACE_EVENT_EXEC << 8);
    return (status == exec_stop);
}

PEEK_EVENT_TYPE peek_treat_wait_status(pid_t pid, int status)
{
    if (pid == -1) {
        if (errno != ECHILD) {
            return PEEK_AGAIN;
        }
    } else if (is_fork_stop(status)) {
        return PEEK_FORKSTOP;
    } else if (is_exec_stop(status)) {
        peek_cont_proc(pid);
        return PEEK_AGAIN;
    } else if (is_group_stop(status)) {
        cont_listen_proc(pid);
        return PEEK_AGAIN;
    } else if (is_event_stop(status)) {
        peek_cont_proc(pid);
        return PEEK_AGAIN;
    } else if (WIFEXITED(status)) {
        return PEEK_EXITED;
    } else if (is_syscall_stop(status)) {
        return PEEK_SYSSTOP;
    } else if (WIFSTOPPED(status)) {
        cont_sys_proc(pid, WSTOPSIG(status));
        return PEEK_AGAIN;
    }
    return PEEK_ERROR;
}

pid_t peek_attach_exec_child(char *argv[])
{
    pid_t pid;

    switch ((pid = fork())) {
    case -1:
        return 0;
    case 0:
        kill(getpid(), SIGSTOP);
        execvp(*argv, argv);
        exit(EXIT_FAILURE);
    }
    waitid(P_PID, pid, NULL, WSTOPPED);
    if (attach_running_proc(pid) == -1) {
        return 0;
    }
    kill(pid, SIGCONT);
    return 0;
}

void peek_treat_fork(pid_t parent)
{
    pid_t new = peek_get_new_proc_pid(parent);
    peek_cont_proc(parent);
    waitid(P_PID, new, NULL, WSTOPPED);
    peek_cont_proc(new);
}

void peek_cont_proc(pid_t pid)
{
    cont_sys_proc(pid, 0);
}

pid_t peek_attach_running_proc(pid_t pid)
{
    return attach_running_proc(pid);
}

int peek_syscall_info(syscall_info_t *si, pid_t pid)
{
    struct ptrace_syscall_info d;
    if (-1  == ptrace(PTRACE_GET_SYSCALL_INFO, pid, sizeof(d), &d)) {
        return -1;
    }
    return nrsi_update_from_ptrace_info(si, pid, &d);
}

