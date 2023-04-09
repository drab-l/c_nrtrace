#include <util_header/stdc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "tracer.h"

noreturn_ static void print_usage(const char *proc)
{
    printf(
"trace and print tracee syscall\n"
"Usage:\n"
"    %s [Option] [CMD [ARGs...]]\n"
"\n"
"Option:\n"
"    -p  : Trace target thread ids, separated comma. Don't trace other thread in same process.\n"
"    -e  : Print syscall names, separated comma. Default is all print.\n"
"    --ee: Print syscall inclusive names, separated comma. Default is all print.\n"
"    -E  : No print syscall names, separated comma.\n"
"    --EE: No print syscall names, separated comma.\n"
"    -s  : Simple print syscalll names, separated comma.\n"
"    --ss: Simple print syscall inclusive names, separated comma.\n",
    proc);
    exit(EXIT_SUCCESS);
}

static int get_str_opt(const char *opt, char **argv, char **r)
{
    char **v = argv;
    size_t l = strlen(opt);
    int rr = 0;
    if (!strncmp(*v, opt, l)) {
        if ((*v)[l]) {
            *r = &(*v)[l];
            rr = 1;
        } else if (++v) {
            *r = *v;
            rr = 2;
        } else {
            rr = -1;
        }
    }
    return rr;
}

static void handle_pid_opt(long p)
{
    if (p > 0) {
        tracer_attach_running_proc(p);
    }
}

static void handle_nums_opt(const char *opt, void(*cb)(long))
{
    char o[strlen(opt) + 1];
    char *s = o;
    memcpy(o, opt, sizeof(o));
    while ((s = strtok(s, ","))) {
        char *e;
        long n = strtol(s, &e, 0);
        if (s != e) {
            cb(n);
        }
        s = NULL;
    }
}

static void handle_unset_opt_pre()
{
    static int called = 0;
    if (!called) {
        called = 1;
        tracer_set_no_print();
    }
}

static void handle_ss_opt(const char *e)
{
    tracer_set_print_simple_by_include_name(e);
}

static void handle_s_opt(const char *e)
{
    tracer_set_print_simple_by_name(e);
}

static void handle_E_opt(const char *e)
{
    tracer_set_print_skip_by_name(e);
}

static void handle_EE_opt(const char *e)
{
    tracer_set_print_skip_by_include_name(e);
}

static void handle_e_opt(const char *e)
{
    handle_unset_opt_pre();
    tracer_unset_print_skip_by_name(e);
}

static void handle_ee_opt(const char *e)
{
    handle_unset_opt_pre();
    tracer_unset_print_skip_by_include_name(e);
}

static void handle_strs_opt(const char *opt, void(*cb)(const char*))
{
    char o[strlen(opt) + 1];
    memcpy(o, opt, sizeof(o));
    for (size_t i = 0; i <= sizeof(o); i++) {
        if (o[i] == ',') {
            o[i] = '\0';
        }
    }
    for (size_t i = 0; i <= sizeof(o); ) {
        cb(&o[i]);
        while (i <= sizeof(o) && o[i]) i++;
        while (i <= sizeof(o) && !o[i]) i++;
    }
}

static void handle_opt_stage(char **argv, unsigned int stage)
{
    const char *comm = argv[0];
    char **v = &argv[1];
    char *out = NULL;
    while (*v) {
        int r;
        char *s;
        if ((r = get_str_opt("-p", v, &s)) < 0) {
            print_usage(comm);
        } else if (r > 0) {
            v += r;
            if (stage == 1) { handle_nums_opt(s, handle_pid_opt); }
        } else if ((r = get_str_opt("-o", v, &s)) < 0) {
            print_usage(comm);
        } else if (r > 0) {
            v += r;
            if (stage == 0) { out = s; }
        } else if ((r = get_str_opt("-e", v, &s)) < 0) {
            print_usage(comm);
        } else if (r > 0) {
            v += r;
            if (stage == 0) { handle_strs_opt(s, handle_e_opt); }
        } else if ((r = get_str_opt("--ee", v, &s)) < 0) {
            print_usage(comm);
        } else if (r > 0) {
            v += r;
            if (stage == 0) { handle_strs_opt(s, handle_ee_opt); }
        } else if ((r = get_str_opt("-E", v, &s)) < 0) {
            print_usage(comm);
        } else if (r > 0) {
            v += r;
            if (stage == 0) { handle_strs_opt(s, handle_E_opt); }
        } else if ((r = get_str_opt("--EE", v, &s)) < 0) {
            print_usage(comm);
        } else if (r > 0) {
            v += r;
            if (stage == 0) { handle_strs_opt(s, handle_EE_opt); }
        } else if ((r = get_str_opt("-s", v, &s)) < 0) {
            print_usage(comm);
        } else if (r > 0) {
            v += r;
            if (stage == 0) { handle_strs_opt(s, handle_s_opt); }
        } else if ((r = get_str_opt("--ss", v, &s)) < 0) {
            print_usage(comm);
        } else if (r > 0) {
            v += r;
            if (stage == 0) { handle_strs_opt(s, handle_ss_opt); }
        } else {
            break;
        }
    }

    if (stage == 0) {
        FILE *fp = out ? fopen(out, "wbe") : NULL;
        tracer_set_output(fp ? fp : stderr);
    } else if (stage == 1) {
        if (*v) {
            tracer_attach_exec_child(v);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        print_usage(argv[0]);
    }

    handle_opt_stage(argv, 0);
    handle_opt_stage(argv, 1);

    tracer_start();
    return 0;
}
