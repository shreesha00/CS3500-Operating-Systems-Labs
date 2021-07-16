#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    char* arguments[argc-1];
    int i = 0;
    for(i=1;i<argc;i++)
    {
        arguments[i-1] = argv[i];
    }
    if(echo_kernel(argc - 1, arguments) < 0)
    {
        printf("unsuccessful system call\n");
        exit(1);
    }
    exit(0);
}