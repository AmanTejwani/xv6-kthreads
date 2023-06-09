#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_clone(void)
{
  int (*func)(void *);
  char *childStack;
  int flags;
  char *args;
  if( argptr(0,(char **)&func,sizeof(void *)) < 0 || argint(1,(int *)&childStack) <0 || argint(2,&flags)<0 || argptr(3,(char **)&args,sizeof(void *) < 0))
      return -1;

  return clone(func,childStack,flags,args);
}

int
sys_join(void)
{
  int pid;
  if(argint(0,&pid)<0)
    return -1;
  return join(pid);
}

int
sys_tkill(void)
{
  int tid;
  if(argint(0,&tid)<0)
    return -1;
  return tkill(tid);
}

int
sys_tgkill(void)
{
  return tgkill();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->tgid;
}

int
sys_gettid(void)
{
  return myproc()->pid;
}

int
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

int
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

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
