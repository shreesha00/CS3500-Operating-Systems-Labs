#include "kernel/types.h"
#include "kernel/memlayout.h"
#include "user/user.h"


int main()
{
    int *a = (int*)malloc(1000000*sizeof(int));
    for(int i=0; i<1000000; i++) a[i] = 2;
    int pid = fork();
    if(pid > 0)
    {
        wait(&a[1]);
    }
    else
    {
        exit(0);
    }
    if(a[1] == 0)
    {
        printf("Ok\n");
    }
    exit(0);
}