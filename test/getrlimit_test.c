#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

typedef unsigned long long ull;
int main(int argc, char **argv)
{
    (void)argc, (void)argv;
    struct rlimit r = {};
    getrlimit(RLIMIT_AS, &r);
    write(1,"1",1);
    return 0;
}
