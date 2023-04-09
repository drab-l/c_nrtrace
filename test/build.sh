#!/bin/bash -e
main () {
local CUR=$(dirname $0)
local SRC="${CUR}/${1}_test.c"
local BIN64="${CUR}/bin/${1}.64.out"
local BIN32="${CUR}/bin/${1}.32.out"
shift 1
clang -std=gnu2x "${SRC}" -o "${BIN64}" "$@"
clang -std=gnu2x "${SRC}" -o "${BIN32}" "$@" -m32
}

main "$@"
