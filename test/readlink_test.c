#include <unistd.h>

int main(int argc, char **argv)
{
    char buf[512];
    (void)readlink(argc <= 1 ? "." : argv[1], buf, sizeof(buf));
    return 0;
}
