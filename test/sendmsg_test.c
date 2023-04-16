#define _GNU_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>

static void recver(int sd)
{
    struct msghdr msg = { 0 };
    pid_t pid[2] = {};
    struct iovec io = {
        .iov_base = pid,
        .iov_len = sizeof(pid)
    };
    _Alignas(8) char buf[CMSG_SPACE(sizeof(int)) + CMSG_SPACE(sizeof(struct ucred))];
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    (void)recvmsg(sd, &msg, 0);
}
static void sender(int sd)
{
    struct msghdr msg = { 0 };
    pid_t pid = getpid();
    struct iovec io = {
        .iov_base = &pid,
        .iov_len = sizeof(pid)
    };
    int fd = STDIN_FILENO;
    struct ucred c = {getpid(), getuid(), getgid()};
    union {
        char buf[CMSG_SPACE(sizeof(fd)) + CMSG_SPACE(sizeof(c))];
        struct cmsghdr align;
    } u = {};
    char *buf = u.buf;

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(u);

    struct cmsghdr *next, *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd));
    memcpy(CMSG_DATA(cmsg), &fd, sizeof(fd));

    next = CMSG_NXTHDR(&msg, cmsg);
    cmsg = next;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_CREDENTIALS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(c));
    memcpy(CMSG_DATA(cmsg), &c, sizeof(c));
    sendmsg(sd, &msg, 0);
}

int main()
{
    int sv[2];
    socketpair(AF_LOCAL, SOCK_SEQPACKET, 0, sv);
    if (fork()) {
        sender(sv[0]);
    } else {
        recver(sv[1]);
    }
    return 0;
}

