#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#define STACK_SIZE 4096
#define CLONE_VM   1
#define CLONE_FILES   4
#define CLONE_THREAD 8
char *argv[] = { "clonethread", 0 };
int y=15;
struct temp{
    int a;
    int b;
};
int func_args(void *args)
{
    struct temp *var = (struct temp *)args;
    if((var->a*var->b)==6)
        printf(1,"Multi  Arg test pass \n");
    else
        printf(1,"Multi Arg test fail \n");
    exit();
}

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
    exec("clonethread", argv);
    int x=gettid();
    int y=getpid();
    printf(1,"value of tid is %d and value of pid/tgid is  %d \n",x,y);
    exit();
}
int foo(void *arg){
    int args=6;
    char *childStack=(char *)malloc(STACK_SIZE);
    int flags=CLONE_THREAD;
    int tid=clone(bar,childStack+STACK_SIZE,flags,&args);
    sleep(5);
    join(tid);
    exit();
}
void clonevm(){
    int tid;
    char *childStack=(char *)malloc(STACK_SIZE);
    int args=5;
    int flags=CLONE_VM;
    tid=clone(func,childStack+STACK_SIZE,flags,&args);
    sleep(5);
    if(y==16){
        printf(1," CLONE_VM  test passed\n",y);
    }
    else
        printf(1," CLONE_VM  test failed\n",y);
    join(tid);
    return;
}
void mutiple_Arg(){
    char *childStack=(char *)malloc(STACK_SIZE);
    struct temp *tmp = (struct temp *)malloc(sizeof(struct temp));
    tmp->a = 2;
    tmp->b = 3;
    int tid=clone(func_args,childStack+STACK_SIZE,CLONE_VM,tmp);
    join(tid);
    return;
}
void cloneFile(){
    char *thirdchildStack=(char *)malloc(STACK_SIZE);
    int flags=CLONE_FILES;
    int fd = open("test.txt",O_CREATE | O_RDWR);
    int tid=clone(func_CLONE_FILES,thirdchildStack+STACK_SIZE,flags,&fd);
    sleep(5);
    int z = lseek(fd,0,SEEK_CUR);
    if(z == 5)
        printf(1, "CLONE_FILES passed\n");
    else
        printf(1, "CLONE_FILES failed\n");
    join(tid);
    return;
}
void clonethread(){
    int tid;
    char *newChildStack=(char *)malloc(STACK_SIZE);
    int flags=CLONE_THREAD;
    int args=5;
    tid=clone(foo,newChildStack+STACK_SIZE,flags,&args);
    sleep(5);
    join(tid);
}

int main(){
    clonevm();
    mutiple_Arg();
    cloneFile();
    clonethread();
    exit();
}
