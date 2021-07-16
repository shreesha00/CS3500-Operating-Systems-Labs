#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/processinfo.h"

int main()
{
    struct processinfo* p;
    p = (struct processinfo*) malloc(sizeof(struct processinfo));
    if(get_process_info(p) < 0)
    {
        printf("unsuccessful system call\n");
        exit(1);
    }
    printf("Process ID -> %d\n", p->pid);
    printf("Process Name -> %s\n", p->name);
    printf("Memory Size -> %d Bytes\n", p->sz);
    exit(0);
}