#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "processinfo.h"
#define NEWMAXCHARS 200
uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//simple echo system call for lab
uint64
sys_echo_simple(void)
{
  char string[NEWMAXCHARS];
  if(argstr(0, string, NEWMAXCHARS) < 0)
  {
    return -1;
  }
  printf("%s\n", string);
  return 1;
}

//echo kernel system call for lab
uint64
sys_echo_kernel(void)
{
  char string[NEWMAXCHARS];
  int n;
  if(argint(0, &n) < 0)
  {
    return -1;
  }
  uint64 p;
  if(argaddr(1, &p) < 0)
  {
    return -1;
  }

  int i = 0;
  uint64 startaddr;
  for(i=0;i<n;i++)
  {
    if(fetchaddr(p, &startaddr) < 0)
    {
      return -1;
    }
    if(fetchstr(startaddr, string, NEWMAXCHARS) < 0)
    {
      return -1;
    }
    printf("%s ", string);
    p = p + sizeof(uint64);
  }
  printf("\b \n");
  return 1;
}

//get process info system call for lab
uint64
sys_get_process_info(void)
{
  struct proc* p = myproc();
  struct processinfo pinf;

  pinf.pid = p->pid;
  int i = 0;
  for(i=0;i<strlen(p->name);i++)
  {
    pinf.name[i] = p->name[i];
  }
  pinf.name[i] = '\0';
  pinf.sz = p->sz;

  uint64 addr;
  if(argaddr(0, &addr) < 0)
  {
    return -1;
  }  
  if(copyout(p->pagetable, addr, (char*)&pinf, sizeof(pinf)) < 0)
  {
    return -1;
  }
  return 1;
}

//system call tracing
uint64
sys_trace(void)
{
  int tracing;
  if(argint(0, &tracing) < 0)
  {
    return -1;
  }
  struct proc *p = myproc();
  p->tracing = tracing;
  return 1;
}