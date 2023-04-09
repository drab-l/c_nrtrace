#!/bin/sh
make_type_header () {
echo '#pragma once'
sed "s;${1}__NR_;bit${2}__NR_;g" 'cfg/'"${1}"'/NR.h'
echo '#include "'"${1}"'/type.h"'
}

make_table_header () {
echo '#pragma once'
}

make_print_header () {
echo '#pragma once'
echo '#include "'"${1}"'/print.h"'
}

OUT_TYPE="${3}"
OUT_TABL="${4}"
OUT_PRNT="${5}"
OUT_DIR=`dirname "${OUT_TYPE}"`
mkdir -p "$OUT_DIR"

make_type_header "${1}" "${2}" > "${OUT_TYPE}"
make_table_header "${1}" "${2}" > "${OUT_TABL}"
make_print_header "${1}" "${2}" > "${OUT_PRNT}"
