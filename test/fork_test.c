#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/syscall.h>   /* SYS_xxx の定義用 */

#define print_pid() printf("%s:%d %d\n", __func__, getpid(), (int)syscall(SYS_gettid))

static void p()
{
    print_pid();
}

static void cc()
{
    print_pid();
    exit(0);
}

static void c()
{
    print_pid();
}

static void *start(void *) {
    pid_t pid;
    if (!(pid = fork())) {
        c();
        exit(0);
    }
    p();
    waitpid(pid, NULL, 0);
    return NULL;
}

int main() {
    pthread_t t;
    pthread_create(&t, NULL, start, NULL);
    start(NULL);
    return 0;
}
