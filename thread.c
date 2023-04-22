#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread.h"
struct threadLib *head,*tail;

void initlock(struct ticketLock *lk)
{
    lk->ticketNumber = 0;
}
static inline int fetch_and_add(int* variable, int value)
{
    __asm__ volatile("lock; xaddl %0, %1"
      : "+r" (value), "+m" (*variable) // input + output
      : // No input-only
      : "memory"
    );
    return value;
}
void ticketLock_acquire(struct ticketLock *lk)
{
    while (fetch_and_add(&lk->ticketNumber, 1) != 0)
        ;
    __sync_synchronize();
}
void ticketLock_release(struct ticketLock *lk)
{
    __sync_synchronize();
    lk->ticketNumber = 0;
}
void thread_Add(threadLib *th){
    if(!head){
        head=th;
        tail=th;
    }
    else{
        tail->next=th;
        th->prev=tail;
        tail=th;
    }
}
void thread_delete(int tid){
    threadLib *p=head,*q=0;
    while(p->tid!=tid && p->next){
        q=p;
        p=p->next;
    }
    if(p==head){
        head=p->next;
        free(p->stack-4096);
        free(p);
    }
    else{
        q->next=p->next;
        p->next->prev=q;
        if(p==tail){
            tail=q;
        }
        free(p->stack-4096);
        free(p);
    }
}
int thread_create(int (*func)(void *),void *args){
    threadLib* th=(threadLib*)malloc(sizeof(threadLib));
    th->next=0;
    th->prev=0;
    char *stack=(char *)malloc(STACK_SIZE);
    th->arg=(char *)args;
    if(!stack){
        printf(1,"Stack not allocated \n");
        return -1;
    }
    th->stack=stack+4096;
    th->tid=clone(func,th->stack,CLONE_VM |CLONE_THREAD,args);
    sleep(5);
    if(th->tid==-1){
        printf(1,"Clone Failed \n");
        return -1;
    }
    thread_Add(th);
    return th->tid;
}
int thread_join(int tid){
    int retID=join(tid);
    thread_delete(tid);
    return retID;
}
int thread_exit(int tid){
    int x=tkill(tid);
    thread_delete(tid);
    return x;
}