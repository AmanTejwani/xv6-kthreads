#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread.h"

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