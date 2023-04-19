#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#define STACK_SIZE 4096
#define CLONE_THREAD 8

int func2(void *arg){
    printf(1,"In main 1 and value of tid is %d and pid is %d  and value of argument is %d \n",gettid(),getpid(),*(int *)arg);
    int x=gettid();
    int y=getpid();
    printf(1,"value of tid is %d and value of pid/tgid is  %d \n",x,y);
    exit();
}
int func(void *arg){
    printf(1,"In main 1 and value of tid is %d and pid is %d  and value of argument is %d \n",gettid(),getpid(),*(int *)arg);
    int args=6;
    char *childStack=(char *)malloc(STACK_SIZE);
    int flags=CLONE_THREAD;
    int tid=clone(func2,childStack+STACK_SIZE,flags,&args);
    sleep(5);
    int x=join(tid);
    printf(1,"value of x  %d \n",x);
    if(x==tid)
        printf(1,"Joined Successfully \n");
    exit();
}
int main(){
    int args=5;
    int tid;
    char *childStack=(char *)malloc(STACK_SIZE);
    int flags=CLONE_THREAD;
    tid=clone(func,childStack+STACK_SIZE,flags,&args);
    sleep(10);
    int x=join(tid);
    printf(1,"value of x  %d \n",x);
    if(x==tid)
        printf(1,"Joined Successfully \n");
    exit();
}