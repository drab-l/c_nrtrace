#!/bin/sh
make_type_header () {
echo '#pragma once'
echo 'enum {'
sed 's;.*;sys_uni_&,;' 'cfg/syscall_list.h'
echo '};'
echo '/* include arch headers after enum declaration. */'
echo '#include "type_64.h"'
echo '#include "type_32.h"'
echo '#include "uni/bit_type.h"'
}

make_table_header () {
echo '#pragma once'
echo '#include <stddef.h>'
echo '#include "type_uni.h"'
echo '#include "syscall_table_type.h"'
echo 'struct syscall_table {'
echo '    const char *label;'
echo '};'
echo '#define ARG_MAX_ITEM 6'
echo 'struct syscall_print_table {'
echo '    enum syscall_print_type arg_type[ARG_MAX_ITEM];'
echo '    enum syscall_print_type ret_type;'
echo '    int print_skip;'
echo '    int print_simple;'
echo '};'
echo 'int syscall_to_uni_table32(int sysnum);'
echo 'int syscall_to_uni_table64(int sysnum);'
echo 'size_t syscall_table_size(void);'
echo 'const struct syscall_table *syscall_table(void);'
echo 'struct syscall_print_table *syscall_print_table(void);'
}

make_print_header () {
echo '#pragma once'
echo '#include "print_64.h"'
echo '#include "print_32.h"'
}

make_source () {
echo '#include "type_uni.h"'
echo '#include "table_uni.h"'
echo 'int syscall_to_uni_table32(int sysnum) {'
echo 'switch(sysnum) {'
sed -n '\@^#define bit32_@!b;s@^#define bit32__NR_\([^ ]*\) .*$@case bit32__NR_\1:return sys_uni_\1;@;p' "${1}/gen/type_32.h"
echo '} return sys_uni_unknown;'
echo '}'
echo 'int syscall_to_uni_table64(int sysnum) {'
echo 'switch(sysnum) {'
sed -n '\@^#define bit64_@!b;s@^#define bit64__NR_\([^ ]*\) .*$@case bit64__NR_\1:return sys_uni_\1;@;p' "${1}/gen/type_64.h"
echo '} return sys_uni_unknown;'
echo '}'
echo '#define DEFINE_SYSCALL_TABLE_ENTRY(name_) DEFINE_SYSCALL_TABLE_ENTRY_IN(NR_PREFIX, name_)'
echo '#define DEFINE_SYSCALL_TABLE_ENTRY_IN(prefix_, name_) DEFINE_SYSCALL_TABLE_ENTRY_IN_(prefix_, name_)'
echo '#define DEFINE_SYSCALL_TABLE_ENTRY_IN_(prefix_, name_) [prefix_##name_] = { .label = #name_ },'
echo '#define NR_PREFIX sys_uni_'
echo 'static const struct syscall_table syscall_table_uni[] = {'
sed "s;.*;DEFINE_SYSCALL_TABLE_ENTRY(&);" ./cfg/syscall_list.h
echo '};'
echo 'static struct syscall_print_table syscall_print_table_uni[sizeof(syscall_table_uni)/sizeof(syscall_table_uni[0])] = {'
sed "s;__PREFIX__;sys_uni_;" ./cfg/syscall_print_entry.h
echo '};'
echo 'size_t syscall_table_size(void) { return sizeof(syscall_table_uni)/sizeof(syscall_table_uni[0]);};'
echo 'const struct syscall_table *syscall_table(void) { return syscall_table_uni; }'
echo 'struct syscall_print_table *syscall_print_table(void) { return syscall_print_table_uni; }'
echo '#undef NR_PREFIX'
}

OUT_TYPE="${2}"
OUT_TABL="${3}"
OUT_PRNT="${4}"
OUT_SRC="${5}"
OUT_DIR=`dirname "${OUT_TYPE}"`
mkdir -p "$OUT_DIR"
make_type_header "${1}" > "${OUT_TYPE}"
make_table_header "${1}" > "${OUT_TABL}"
make_print_header "${1}" > "${OUT_PRNT}"
make_source "${1}" > "${OUT_SRC}"
