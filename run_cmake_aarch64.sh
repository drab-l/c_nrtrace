#!/bin/sh
cd `dirname $0`
mkdir -p build_aarch64
cd build_aarch64
CC="clang --target=aarch64-linux-gnu" cmake .. &&
cmake -DARCH=aarch64 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON . &&
cmake --build . "${@}"

