#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <linux/audit.h>

#include <util_header/stdc.h>
#include <table_uni.h>
#include "inc/syscall_info.h"

typedef struct syscall_info_t {
    uint64_t nr;
    uint64_t args[6];
    int64_t ret;
    int32_t pid;
    uint16_t uni_nr;
    uint16_t is_bit64:1;
    uint16_t is_exited:1;
    uint16_t is_error:1;
} syscall_info_t;

assert_static_(sizeof(syscall_info_t) == SYSCALL_INFO_SIZE);
assert_static_(_Alignof(syscall_info_t) == SYSCALL_INFO_ALIGN);

assert_static_(sizeof(pid_t) == sizeof(int32_t));
assert_static_((pid_t)-1 == (int32_t)-1);

uint64_t nrsi_sys_num(const syscall_info_t *p) { return p->nr; }
bit_sreg_t nrsi_argR(const syscall_info_t *p) { return p->ret; }
bit_reg_t nrsi_arg1(const syscall_info_t *p) { return p->args[0]; }
bit_reg_t nrsi_arg2(const syscall_info_t *p) { return p->args[1]; }
bit_reg_t nrsi_arg3(const syscall_info_t *p) { return p->args[2]; }
bit_reg_t nrsi_arg4(const syscall_info_t *p) { return p->args[3]; }
bit_reg_t nrsi_arg5(const syscall_info_t *p) { return p->args[4]; }
bit_reg_t nrsi_arg6(const syscall_info_t *p) { return p->args[5]; }
bit_reg_t nrsi_argN(const syscall_info_t *p, enum ARGN n)
{
    return (ARG1 <= n && ARG6 >= n) ? p->args[n] :
        (ARGR == n) ? p->ret : (abort(), 0);
}

void nrsi_set_sys_num(syscall_info_t *p, uint64_t sys) { p->nr = sys; }
void nrsi_set_argR(syscall_info_t *p, bit_sreg_t n) { p->ret = n; }
void nrsi_set_arg1(syscall_info_t *p, bit_reg_t n) { p->args[0] = n; }
void nrsi_set_arg2(syscall_info_t *p, bit_reg_t n) { p->args[1] = n; }
void nrsi_set_arg3(syscall_info_t *p, bit_reg_t n) { p->args[2] = n; }
void nrsi_set_arg4(syscall_info_t *p, bit_reg_t n) { p->args[3] = n; }
void nrsi_set_arg5(syscall_info_t *p, bit_reg_t n) { p->args[4] = n; }
void nrsi_set_arg6(syscall_info_t *p, bit_reg_t n) { p->args[5] = n; }
void nrsi_set_args(syscall_info_t *p, const bit_reg_t n[static 6]) { memcpy(p->args, n, sizeof(p->args)); }

uint32_t nrsi_errno(const syscall_info_t *p) {
    return nrsi_is_error(p) ? (-p->ret & 0xFFF) : 0;
}
int32_t nrsi_pid(const syscall_info_t *p) { return p->pid; }
void nrsi_set_pid(syscall_info_t *p, int32_t pid) { p->pid = pid; }

uint64_t nrsi_uni_nr(const syscall_info_t *p) { return p->uni_nr; }
uint64_t nrsi_set_uni_sys_num(syscall_info_t *p, unsigned int nr) { return p->uni_nr = nr; }

bool nrsi_is_bit64(const syscall_info_t *p) { return p->is_bit64; }
bool nrsi_is_exited(const syscall_info_t *p) { return p->is_exited; }
bool nrsi_is_error(const syscall_info_t *p) { return p->is_error; }

const char *nrsi_sys_label(const syscall_info_t *p) { return syscall_table()[nrsi_uni_nr(p)].label; }

const print_entry_t *nrsi_print_entry(const syscall_info_t *p) { return &syscall_print_table()[nrsi_uni_nr(p)]; }
enum syscall_print_type nrpe_print_type_argN(const print_entry_t *p, enum ARGN n)
{
    return (ARG1 <= n && ARG6 >= n) ? p->arg_type[n] :
        (ARGR == n) ? p->ret_type : (abort(), SYSCALL_PRINT_NONE);
}

void nrsi_copy(syscall_info_t *dst, const syscall_info_t *src) { memcpy(dst, src, sizeof(syscall_info_t)); }

int nrsi_update_from_ptrace_info(syscall_info_t *si, int32_t pid, const struct ptrace_syscall_info *psi)
{
    if (psi->op == PTRACE_SYSCALL_INFO_ENTRY) {
        si->nr = psi->entry.nr;
        memcpy(si->args, psi->entry.args, sizeof(psi->entry.args));
        si->is_exited = false;
    } else if (psi->op == PTRACE_SYSCALL_INFO_EXIT) {
        si->ret = psi->exit.rval;
        si->is_error = psi->exit.is_error;
        si->is_exited = true;
    } else {
        return -1;
    }
    si->pid = pid;
    si->is_bit64 = (psi->arch & __AUDIT_ARCH_64BIT) == __AUDIT_ARCH_64BIT;
    si->uni_nr = si->is_bit64 ? syscall_to_uni_table64(si->nr) : syscall_to_uni_table32(si->nr);
    return 0;
}

enum syscall_print_type nrsi_print_type_argN(const syscall_info_t *si, enum ARGN n)
{
    const print_entry_t *pe = nrsi_print_entry(si);
    enum syscall_print_type spt = nrpe_print_type_argN(pe, n);
    int s = pe->print_simple;
    return (s && (spt != SYSCALL_PRINT_NONE)) ? SYSCALL_PRINT_REG_HEX : spt;

}

enum syscall_print_type nrsi_print_type_argR(const syscall_info_t *si)
{
    const print_entry_t *pe = nrsi_print_entry(si);
    enum syscall_print_type spt = pe->ret_type;
    int s = pe->print_simple;
    return (s && (spt != SYSCALL_PRINT_NONE)) ? SYSCALL_PRINT_REG_HEX : spt;

}

bool nrsi_is_print_skip(const syscall_info_t *si)
{
    const print_entry_t *pe = nrsi_print_entry(si);
    return !!pe->print_skip;
}

bool nrsi_is_print_configured(const syscall_info_t *si)
{
    return nrsi_print_type_argR(si) != SYSCALL_PRINT_NONE;
}

