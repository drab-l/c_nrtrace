#pragma once

#include <sys/types.h>
#include <stdio.h>
void tracer_start(void);
void tracer_attach_running_proc(pid_t pid);
void tracer_set_no_print(void);
void tracer_set_output(FILE *fp);
void tracer_attach_exec_child(char *argv[]);
void tracer_set_print_skip_by_name(const char *name);
void tracer_unset_print_skip_by_name(const char *name);
void tracer_set_print_skip_by_include_name(const char *name);
void tracer_unset_print_skip_by_include_name(const char *name);

void tracer_set_print_simple_by_name(const char *name);
void tracer_set_print_simple_by_include_name(const char *name);
