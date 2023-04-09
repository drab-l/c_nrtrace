#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(int argc, char **argv)
{
#ifdef SYS_getdents64
    long dd[512];
    syscall(SYS_getdents64, argc <= 1 ? open(".", O_RDONLY) : open(argv[1], O_RDONLY), dd, sizeof(dd));
#endif
    return 0;
}
