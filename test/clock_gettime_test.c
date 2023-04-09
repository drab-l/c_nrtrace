#define _GNU_SOURCE
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>

int main(int, char **)
{
    struct timespec tp;
#ifdef SYS_clock_gettime
    syscall(SYS_clock_gettime,CLOCK_MONOTONIC, &tp);
#endif
#ifdef SYS_clock_gettime64
    syscall(SYS_clock_gettime64,CLOCK_MONOTONIC, &tp);
#endif
    return 0;
}
