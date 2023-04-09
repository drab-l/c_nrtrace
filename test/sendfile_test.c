#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

int main(int, char **argv)
{
    int i = open(*argv, O_RDONLY);
    int o = open("/dev/null", O_WRONLY);
    off_t f = 0;
    sendfile(o, i, &f, 10);
    return 0;
}
