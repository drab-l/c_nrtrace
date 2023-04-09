#define _GNU_SOURCE
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>

int main(int, char **)
{
    int fd[2];
    pipe(fd);
    pipe2(fd, 0);
    return 0;
}
