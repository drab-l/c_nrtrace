#define _GNU_SOURCE
#include <unistd.h>
#include <sys/uio.h>

int main(int, char **)
{
    int fd[2];
    pipe(fd);
    char buf[100];
    char s[] = "abc1234567890@saaepoihgewr";
    struct iovec vec[3] = {{buf + 10, 3},{buf, 10},{buf + 13, 87}};
    write(fd[1], s, sizeof(s) - 1);
    ssize_t r = readv(fd[0],vec, 3);
    buf[r] = '\n';
    write(1, buf, r + 1);
    return 0;
}
