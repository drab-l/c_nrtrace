#!/bin/sh
cd `dirname $0`
mkdir -p build
cd build
CC=clang CXX=clang++ cmake .. &&
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON . &&
cmake --build . "${@}"

