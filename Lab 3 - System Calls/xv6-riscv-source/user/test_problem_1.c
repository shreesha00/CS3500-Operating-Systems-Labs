#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    if(argc > 2)
    {
        printf("too many arguments\n");
        exit(1);
    }
    if(echo_simple(argv[1]) < 0)
    {
        printf("unsuccessful system call\n");
        exit(1);
    }
    exit(0);
}
