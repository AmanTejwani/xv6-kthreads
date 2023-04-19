#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#define STACK_SIZE 4096
#define CLONE_THREAD 8

int func(void *arg){
    printf(1,"In main 1 and value of tid is %d and pid is %d  and value of argument is %d \n",gettid(),getpid(),*(int *)arg);
    exit();
}
int main(){
    int args=5;
    int tid;
    char *childStack=(char *)malloc(STACK_SIZE);
    int flags=CLONE_THREAD;
    tid=clone(func,childStack+STACK_SIZE,flags,&args);
    sleep(5);
    printf(1,"In main 1 and value of tid is %d \n",tid);
    int x=join(tid);
    printf(1,"value of x  %d \n",x);
    if(x==tid)
        printf(1,"Joined Successfully \n");
    exit();
}