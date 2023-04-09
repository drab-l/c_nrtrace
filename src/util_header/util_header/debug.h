#pragma once

#include <stdio.h>

#define LINE_2(l_) fputs(#l_ "\n", stderr)
#define LINE_1(l_) LINE_2(l_)
#define LINE() LINE_1(__LINE__)
