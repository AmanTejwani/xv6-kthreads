#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#define STACK_SIZE 4096
#define CLONE_VM   1
#define CLONE_FILES   4
#define CLONE_THREAD 8
int y=15;
int func(void *arg){
    if(*((int*)arg)==5)
        printf(1,"Argument Received Correctly \n");
    else
        printf(1,"Argument Not Received Correctly \n");
    y++;
    exit();
}
int func_CLONE_FILES(void *arg){
    int fd = *(int *)arg;
    lseek(fd, 5, SEEK_CUR);
    exit();
}
int bar(void *arg){
    printf(1,"In main 1 and value of tid is %d and pid is %d  and value of argument is %d \n",gettid(),getpid(),*(int *)arg);
    int x=gettid();
    int y=getpid();
    printf(1,"value of tid is %d and value of pid/tgid is  %d \n",x,y);
    exit();
}
int foo(void *arg){
    printf(1,"In main 1 and value of tid is %d and pid is %d  and value of argument is %d \n",gettid(),getpid(),*(int *)arg);
    int args=6;
    char *childStack=(char *)malloc(STACK_SIZE);
    int flags=CLONE_THREAD;
    int tid=clone(bar,childStack+STACK_SIZE,flags,&args);
    sleep(5);
    int x=join(tid);
    if(x==tid)
        printf(1,"Joined Successfully \n");
    exit();
}
int main(){
    int tid;
    char *childStack=(char *)malloc(STACK_SIZE);
    char *newChildStack=(char *)malloc(STACK_SIZE);
    int args=5;
    int flags=CLONE_VM;
    tid=clone(func,childStack+STACK_SIZE,flags,&args);
    sleep(5);
    if(flags & CLONE_VM){
        if(y==16){
            printf(1," CLONE_VM  test passed\n",y);
        }
        else
            printf(1," CLONE_VM  test failed\n",y);
    }
    int x=join(tid);
    if(x==tid)
        printf(1,"Joined Successfully for tid %d \n",x);
   
    flags=CLONE_THREAD;
    tid=clone(foo,newChildStack+STACK_SIZE,flags,&args);
    sleep(10);
    // flags=CLONE_FILES;
    // int fd = open("test.txt",O_CREATE | O_RDWR);
    // tid=clone(func_CLONE_FILES,newChildStack+STACK_SIZE,flags,&fd);
    // sleep(3);
    // int z = lseek(fd,0,SEEK_CUR);
    // if(z == 5)
    //     printf(1, "CLONE_FILES passed\n");
    // else
    //     printf(1, "CLONE_FILES failed\n");
    x=join(tid);
    if(x==tid)
        printf(1,"Joined Successfully for tid %d \n",x);
    exit();
}
