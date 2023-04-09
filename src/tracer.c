#include <util_header_all.h>
#include <vl_array.h>
#include <syscall_info.h>
#include <string_buffer.h>
#include <print_uni.h>
#include <table_uni.h>
#include <peek.h>
#include <printer.h>

static int update_syscall_info(syscall_info_t *si, pid_t pid)
{
    return peek_syscall_info(si, pid);
}

static FILE *logout = NULL;
static string_buffer *logout_sb;
static void init_string_buffer(FILE *out)
{
    logout = out;
    logout_sb = get_string_buffer();
}

static string_buffer *get_cached_string_buffer(void)
{
    return logout_sb;
}

static void flush_cached_string_buffer(void)
{
    string_buffer_fputs(logout_sb, logout);
    string_buffer_set_empty(logout_sb);
}

static vl_array *trace_history;
static int is_invalid_syscall_info(const void *p)
{
    const syscall_info_t *si = p;
    return !nrsi_pid(si);
}

static int is_same_syscall_info_as_pid(const void *lv, const void *rv)
{
    const syscall_info_t *l = lv;
    int32_t pid = (uintptr_t)rv;
    return nrsi_pid(l) == pid;
}

static void *add_syscall_info(syscall_info_t *si)
{
    if (!trace_history) {
        if (!(trace_history = get_vl_array(sizeof(syscall_info_buf)))) {
            return NULL;
        }
    }
    return searched_or_tail_insert_vl_array_entry(trace_history, si, is_invalid_syscall_info);
}

static void *add_blank_syscall_info(pid_t pid)
{
    syscall_info_buf sib = {};
    syscall_info_t *si = (void*)&sib;
    nrsi_set_pid(si, pid);
    return add_syscall_info(si);
}

static void rm_syscall_info(pid_t pid)
{
    if (!trace_history) {
        return;
    }
    syscall_info_t *si = search_vl_array_entry(trace_history, is_same_syscall_info_as_pid, (void*)(intptr_t)pid);
    if (si) {
        nrsi_set_pid(si, 0);
    }
}

syscall_info_t *search_syscall_info(pid_t pid)
{
    if (!trace_history) {
        return NULL;
    }
    return search_vl_array_entry(trace_history, is_same_syscall_info_as_pid, (void*)(intptr_t)pid);
}

static int print_syscall(pid_t pid)
{
    syscall_info_t *si;
    if (!(si = search_syscall_info(pid))) {
        if (!(si = add_blank_syscall_info(pid))) {
            return 0;
        }
    }
    if (update_syscall_info(si, pid)) {
        return 1;
    }

    printer_print_syscall(get_cached_string_buffer(), si);
    return 0;
}

static void rm_trace_pid(pid_t pid)
{
    rm_syscall_info(pid);
}

static pid_t trace_wait(void)
{
    while (1) {
        int status;
        pid_t pid;
        pid = waitpid(-1, &status, __WALL);
        switch (peek_treat_wait_status(pid, status)) {
        case PEEK_SYSSTOP: return pid;
        case PEEK_ERROR: return -1;
        case PEEK_AGAIN: continue;
        case PEEK_FORKSTOP: peek_treat_fork(pid); continue;
        case PEEK_EXITED: rm_trace_pid(pid); continue;
        }
    }
}

static int print_syscall_and_cont(pid_t pid)
{
    if (pid && print_syscall(pid)) {
        return -1;
    }
    peek_cont_proc(pid);
    flush_cached_string_buffer();
    return 0;
}

static void set_no_print()
{
    static int called = 0;
    if (called) {
        return;
    }
    struct syscall_print_table *table = syscall_print_table();
    for (size_t i = 0; i < syscall_table_size(); i++) {
        table[i].print_skip = 1;
    }
    called = 1;
}

#define set_match_label(name_, cmp_, member_, val_) do {\
    struct syscall_print_table *pr = syscall_print_table(); \
    const struct syscall_table *tbl = syscall_table(); \
    for (size_t i_ = 0; i_ < syscall_table_size(); i_++) { \
        if (tbl[i_].label && cmp_(tbl[i_].label, name_)) { \
            pr[i_].member_ = val_; \
        } \
    } \
} while (0)

#define set_include_label(name_, member_, val_) set_match_label(name_, is_include_name, member_, val_)
#define set_equal_label(name_, member_, val_) set_match_label(name_, is_equal_name, member_, val_)

static int is_include_name(const char *label, const char *name) { return !!strstr(label, name); }
static int is_equal_name(const char *label, const char *name) { return !strcmp(label, name); }

static void set_print_skip_by_name(const char *name, int val)
{
    set_equal_label(name, print_skip, val);
}

static void set_print_skip_by_include_name(const char *name, int val)
{
    set_include_label(name, print_skip, val);
}

static void set_print_simple_by_name(const char *name, int val)
{
    set_equal_label(name, print_simple, val);
}

static void set_print_simple_by_include_name(const char *name, int val)
{
    set_include_label(name, print_simple, val);
}

static void handler(int)
{
    _exit(EXIT_SUCCESS);
}

void tracer_start()
{
    signal(SIGINT, handler);
    pid_t pid = 0;
    while ((pid = trace_wait()) != -1) {
        if (print_syscall_and_cont(pid)) {
            break;
        }
    }
}

void tracer_attach_running_proc(pid_t pid)
{
    peek_attach_running_proc(pid);
    if (print_syscall_and_cont(pid)) {
        exit(EXIT_FAILURE);
    }
}

void tracer_set_no_print()
{
    set_no_print();
}

void tracer_set_output(FILE *fp)
{
    init_string_buffer(fp ? fp : stderr);
}

void tracer_attach_exec_child(char *argv[])
{
    peek_attach_exec_child(argv);
}

void tracer_set_print_skip_by_name(const char *name)
{
    set_print_skip_by_name(name, 1);
}

void tracer_unset_print_skip_by_name(const char *name)
{
    set_print_skip_by_name(name, 0);
}

void tracer_set_print_skip_by_include_name(const char *name)
{
    set_print_skip_by_include_name(name, 1);
}

void tracer_unset_print_skip_by_include_name(const char *name)
{
    set_print_skip_by_include_name(name, 0);
}

void tracer_set_print_simple_by_name(const char *name)
{
    set_print_simple_by_name(name, 1);
}

void tracer_set_print_simple_by_include_name(const char *name)
{
    set_print_simple_by_include_name(name, 1);
}

