#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

typedef unsigned long long ull;
int main(int argc, char **argv)
{
#ifdef SYS_stat64
    char d[512], *b = d;
    struct stat s;
    stat(argc <= 1 ? "." : argv[1], &s);
    b += sprintf(b, "{.st_dev = %#llx, .st_ino = %llu, .st_mode = %#llx, .st_nlink = %llu, .st_uid = %llu, .st_gid = %llu, .st_rdev = %llu, .st_size = %llu, .st_blksize = %llu, .st_blocks = %llu, ",
                (ull)s.st_dev, (ull)s.st_ino, (ull)s.st_mode, (ull)s.st_nlink, (ull)s.st_uid, (ull)s.st_gid, (ull)s.st_rdev, (ull)s.st_size, (ull)s.st_blksize, (ull)s.st_blocks);
    b += sprintf(b, ".st_atim = {.tv_sec = %llu, .tv_nsec = %llu}, ", (ull)s.st_atim.tv_sec, (ull)s.st_atim.tv_nsec);
    b += sprintf(b, ".st_mtim = {.tv_sec = %llu, .tv_nsec = %llu}, ", (ull)s.st_mtim.tv_sec, (ull)s.st_mtim.tv_nsec);
    b += sprintf(b, ".st_ctim = {.tv_sec = %llu, .tv_nsec = %llu}", (ull)s.st_ctim.tv_sec, (ull)s.st_ctim.tv_nsec);
    puts(d);
#else
    (void)argc, (void)argv;
#endif
    return 0;
}
