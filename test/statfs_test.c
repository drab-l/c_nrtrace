#include <sys/vfs.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    struct statfs s;
    statfs(argc <= 1 ? "." : argv[1], &s);

    printf("{.f_type = %#llx, .f_bsize = %llu, .f_blocks = %llu, .f_bfree = %llu, .f_bavail = %llu, .f_files = %llu, .f_ffree = %llu, .f_fsid = {%d, %d}, .f_namelen = %llu, .f_frsize = %llu, .flags = %#llx}\n",
                (unsigned long long)s.f_type, (unsigned long long)s.f_bsize, (unsigned long long)s.f_blocks, (unsigned long long)s.f_bfree, (unsigned long long)s.f_bavail, (unsigned long long)s.f_files, (unsigned long long)s.f_ffree, s.f_fsid.__val[0], s.f_fsid.__val[1], (unsigned long long)s.f_namelen, (unsigned long long)s.f_frsize, (unsigned long long)s.f_flags);
    return 0;
}
