#pragma once
#include <string_buffer.h>
#include <type_uni.h>
#include <print_uni.h>

#include "printer.h"
#
#ifndef bit64_pollfd
typedef struct {bit64_sint fd; bit64_sshort events; bit64_sshort revents;} bit64_pollfd;
#endif
#ifndef bit32_pollfd
typedef struct {bit32_sint fd; bit32_sshort events; bit32_sshort revents;} bit32_pollfd;
#endif
