#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

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
  //backtrace();
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

uint64
sys_sigalarm(void)
{
  struct proc* p = myproc();
  if((argint(0, &(p->ticks)) < 0) || (argaddr(1, &(p->handler)) < 0))
  {
    return -1;
  }
  p->ticks_passed = 0;
  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc* p = myproc();
  p->tf->ra = p->ra;
  p->tf->sp = p->sp;
  p->tf->gp = p->gp;
  p->tf->tp = p->tp;
  p->tf->t0 = p->t0;
  p->tf->t1 = p->t1;
  p->tf->t2 = p->t2;
  p->tf->s0 = p->s0;
  p->tf->s1 = p->s1;
  p->tf->a0 = p->a0;
  p->tf->a1 = p->a1;
  p->tf->a2 = p->a2;
  p->tf->a3 = p->a3;
  p->tf->a4 = p->a4;
  p->tf->a5 = p->a5;
  p->tf->a6 = p->a6;
  p->tf->a7 = p->a7;
  p->tf->s2 = p->s2;
  p->tf->s3 = p->s3;
  p->tf->s4 = p->s4;
  p->tf->s5 = p->s5;
  p->tf->s6 = p->s6;
  p->tf->s7 = p->s7;
  p->tf->s8 = p->s8;
  p->tf->s9 = p->s9;
  p->tf->s10 = p->s10;
  p->tf->s11 = p->s11;
  p->tf->t3 = p->t3;
  p->tf->t4 = p->t4;
  p->tf->t5 = p->t5;
  p->tf->t6 = p->t6;
  p->tf->epc = p->epc;
  p->in_handler = 0;
  p->ticks_passed = 0;
  return 0;
}