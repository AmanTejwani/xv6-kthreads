#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread.h"

int thread_create(k_Thread *thread, int (*func)(void *),void *args){
    int tid;
    thread->stack=(char *)malloc(PGSIZE);
    if(!thread->stack){
        printf(1,"Stack no allocated \n");
        return -1;
    }
    thread->args=args;
    tid=clone(func,thread->stack,CLONE_VM,args);
    sleep(5);
    if(tid==-1){
        printf(1,"Clone Failed \n");
        free(thread->stack);
        thread->stack=0;
        thread->tid=0;
        thread->args=0;
        return -1;
    }
    return tid;
}

int thread_join(k_Thread *thread){
    int retID=join(thread->tid);
    // printf(1,"retID for join is %d \n",retID);
    free(thread->stack);
    thread->stack=0;
    thread->tid=0;
    thread->args=0;
    return retID;
}
