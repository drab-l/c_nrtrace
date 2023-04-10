#pragma once

#include <stddef.h>
#include <sys/ptrace.h>
#include <linux/ptrace.h>

#include <util_header/stdc.h>
#include <type_uni.h>

typedef struct syscall_info_t syscall_info_t;
#define SYSCALL_INFO_SIZE 72
#define SYSCALL_INFO_ALIGN 8

enum ARGN { ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARGR };

typedef struct {
    uint8_t _Alignas(SYSCALL_INFO_ALIGN) data[SYSCALL_INFO_SIZE];
} syscall_info_buf;

assert_static_(_Alignof(uint64_t) == SYSCALL_INFO_ALIGN);
assert_static_(sizeof(syscall_info_buf) == SYSCALL_INFO_SIZE);

#define host_bit64 (sizeof(void*) == 8)
#define host_bit_match(u_) (host_bit64 == (u_).bit64)

uint64_t nrsi_sys_num(const syscall_info_t *d);
bit_sreg_t nrsi_argR(const syscall_info_t *d);
bit_reg_t nrsi_arg1(const syscall_info_t *d);
bit_reg_t nrsi_arg2(const syscall_info_t *d);
bit_reg_t nrsi_arg3(const syscall_info_t *d);
bit_reg_t nrsi_arg4(const syscall_info_t *d);
bit_reg_t nrsi_arg5(const syscall_info_t *d);
bit_reg_t nrsi_arg6(const syscall_info_t *d);
bit_reg_t nrsi_argN(const syscall_info_t *p, unsigned int n);

void nrsi_set_sys_num(syscall_info_t *d, uint64_t sys);
void nrsi_set_argR(syscall_info_t *d, bit_sreg_t n);
void nrsi_set_arg1(syscall_info_t *d, bit_reg_t n);
void nrsi_set_arg2(syscall_info_t *d, bit_reg_t n);
void nrsi_set_arg3(syscall_info_t *d, bit_reg_t n);
void nrsi_set_arg4(syscall_info_t *d, bit_reg_t n);
void nrsi_set_arg5(syscall_info_t *d, bit_reg_t n);
void nrsi_set_arg6(syscall_info_t *d, bit_reg_t n);
void nrsi_set_args(syscall_info_t *p, const bit_reg_t n[static 6]);

uint32_t nrsi_errno(const syscall_info_t *p);

int32_t nrsi_pid(const syscall_info_t *p);
void nrsi_set_pid(syscall_info_t *p, int32_t pid);

uint64_t nrsi_uni_nr(const syscall_info_t *p);
uint64_t nrsi_set_uni_sys_num(syscall_info_t *p, unsigned int nr);

bool nrsi_is_bit64(const syscall_info_t *p);
bool nrsi_is_exited(const syscall_info_t *p);
bool nrsi_is_error(const syscall_info_t *p);

const char *nrsi_sys_label(const syscall_info_t *p);

typedef struct syscall_print_table print_entry_t;
const print_entry_t *nrsi_print_entry(const syscall_info_t *p);
enum syscall_print_type nrpe_print_type_argN(const print_entry_t *p, unsigned int n);

void nrsi_copy(syscall_info_t *dst, const syscall_info_t *src);
int nrsi_update_from_ptrace_info(syscall_info_t *si, int32_t pid, const struct ptrace_syscall_info *psi);

enum syscall_print_type nrsi_print_type_argN(const syscall_info_t *si, enum ARGN n);
enum syscall_print_type nrsi_print_type_argR(const syscall_info_t *si);
bool nrsi_is_print_skip(const syscall_info_t *si);
bool nrsi_is_print_configured(const syscall_info_t *si);
