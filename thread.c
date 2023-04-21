#include "types.h"
#include "stat.h"
#include "user.h"
#include "thread.h"
int thread_create(int (*func)(void *),void *args){
    int tid;
    char *stack=(char *)malloc(STACK_SIZE);
    if(!stack){
        printf(1,"Stack no allocated \n");
        return -1;
    }
    tid=clone(func,stack,CLONE_VM,args);
    sleep(5);
    if(tid==-1){
        printf(1,"Clone Failed \n");
        return -1;
    }
    return tid;
}
int thread_join(int tid){
    int retID=join(tid);
    return retID;
}
