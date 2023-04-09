#define _GNU_SOURCE
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stddef.h>

#define EXIT_IF_EQ(r_,e_) (((r_) != (e_)) || (exit(EXIT_FAILURE),1))
#define EXIT_IF_NEG_ONE(e_) EXIT_IF_EQ(-1, e_)

static int get_bind_or_connect_socket(int is_bind, int flag)
{
    int sock;
    struct sockaddr_un addr;

    EXIT_IF_NEG_ONE(sock = socket(AF_UNIX, flag|SOCK_CLOEXEC, 0));
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
#define SOCKET_PAH "\0sendmsg_socket"
    memcpy(addr.sun_path, SOCKET_PAH, sizeof(SOCKET_PAH));
    EXIT_IF_NEG_ONE(is_bind ?
        (bind(   sock, (struct sockaddr *)&addr, offsetof(struct sockaddr_un, sun_path) + sizeof(SOCKET_PAH))) :
        (connect(sock, (struct sockaddr *)&addr, offsetof(struct sockaddr_un, sun_path) + sizeof(SOCKET_PAH)))
    );
    return sock;
}

static int get_bind_socket(int flag)
{
    return get_bind_or_connect_socket(1, flag);
}

static int get_connect_socket(int flag)
{
    return get_bind_or_connect_socket(0, flag);
}

static void init_iov(struct iovec *iov, void *data, size_t len)
{
    iov->iov_base = data;
    iov->iov_len = len;
}

static void init_msghdr(struct msghdr *msg, struct iovec *iov, size_t iov_len, void *data, size_t data_len, int flags)
{
    *msg = (struct msghdr){.msg_iov = NULL};
    msg->msg_iov = iov;
    msg->msg_iovlen = iov_len;
    msg->msg_control = data;
    msg->msg_controllen = data_len;
    msg->msg_flags = flags;
}

static void recvmsg_fd_and_data(int sock, int *fd, void *data, size_t len)
{
    struct iovec iov;
    struct msghdr msg;
    char buf[CMSG_SPACE(sizeof(*fd))];

    init_iov(&iov, data, len);
    init_msghdr(&msg, &iov, 1, buf, sizeof(buf), MSG_WAITALL);

    EXIT_IF_NEG_ONE(recvmsg(sock, &msg, 0));

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    *fd = *((int *)CMSG_DATA(cmsg));
}

static void sendmsg_fd_and_data(int sock, int fd, void *data, size_t len)
{
    struct iovec iov;
    struct msghdr msg;
    char buf[CMSG_SPACE(sizeof(fd))];
    init_iov(&iov, data, len);
    init_msghdr(&msg, &iov, 1, buf, sizeof(buf), 0);

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    *((int*)CMSG_DATA(cmsg)) = fd;

    EXIT_IF_NEG_ONE(sendmsg(sock, &msg, 0));
}

static void client_send_fd_and_data(int fd, void *data, size_t size)
{
    int sock = get_connect_socket(SOCK_DGRAM);
    sendmsg_fd_and_data(sock, fd, data, size);
    close(sock);
}

static void client_send(void)
{
    client_send_fd_and_data(STDIN_FILENO, &(pid_t){getpid()}, sizeof(pid_t));
}

void sig_child(int num)
{
    (void)num;
    _exit(0);
}

static void server_recv(void)
{
    pid_t pid;
    int fd, sock = get_bind_socket(SOCK_DGRAM);
    recvmsg_fd_and_data(sock, &fd, &pid, sizeof(pid));
    printf("file descriptor %d from pid %d\n", fd, pid);
    while (splice(fd, NULL, STDOUT_FILENO, NULL, 4096, 0) > 0);
    close(fd);
}

int main()
{
    switch(fork()) {
    default :
        signal(SIGCHLD, sig_child);
        server_recv();
        return -1;
    case 0 :
        client_send();
        return 1;
    case -1 :
        return 1;
    }
    return 0;
}
