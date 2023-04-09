#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/epoll.h>
#include <stdlib.h>

typedef unsigned long long ull;
int main(int argc, char **argv)
{
#define EVENT_NUM 3
    (void)argc, (void)argv;
    int efd = epoll_create(EVENT_NUM);
    {
        struct epoll_event e;
        e.events = EPOLLIN;
        e.data.fd = STDIN_FILENO;
        epoll_ctl(efd, EPOLL_CTL_ADD, e.data.fd, &e);
    }
    while (1) {
        struct epoll_event e[EVENT_NUM];
        int n = epoll_wait(efd, e, EVENT_NUM, -1);
        if (n == -1) {
            perror("epoll_pwait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < n; i++) {
            if (e[i].data.fd == STDIN_FILENO) {
                char b[4096];
                size_t r = read(STDIN_FILENO, b, sizeof(b));
                write(STDOUT_FILENO, b, r);
                return 0;
            }
        }
    }
    return 0;
}
