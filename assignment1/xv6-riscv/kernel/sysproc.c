#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "procstat.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  myproc()->etime = ticks;
  release(&tickslock);
  exit(n);
  return 0;  // not reached
}

int
sys_getpid(void)
{
  return getpid();
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

uint64
sys_getppid(void)
{
  if(!myproc()->parent)
  {
    return -1;
  }
  return myproc()->parent->pid;
}

uint64
sys_yield(void)
{
  struct proc* p = myproc();
  acquire(&p->lock);
  p->state = RUNNABLE;
  sched();
  release(&p->lock);
  return 0;
}

uint64
sys_getpa(void)
{
  uint64 va, pa;
  argaddr(0, &va);
  pa = walkaddr(myproc()->pagetable, va) + (va & (PGSIZE-1));
  return pa;
}

uint64
sys_waitpid(void)
{
  uint64 ptr;
  int id;

  argint(0, &id);
  argaddr(0, &ptr);

  return waitpid(id,ptr);
}

int
sys_forkf(void)
{
  int f;
  argint(0,&f);
  return forkf(f);
}

uint64
sys_ps(void){
  return ps();
}

uint64
sys_pinfo(void)
{
  int pid;
  uint64 p;
  argint(0,&pid);
  argaddr(0,&p);
  return pinfo(pid,p);
}