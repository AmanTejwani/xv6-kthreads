#include "types.h"
#include "stat.h"
#include "user.h"
#define STACK_SIZE 4096
#define CLONE_VM   1
int y=15;
int func(void *arg){
    if(*((int*)arg)==5)
        printf(1,"Argument Received Correctly \n");
    else
        printf(1,"Argument Received Correctly \n");
    y++;
    exit();
}
int main(){
    int tid;
    int args=5;
    char *childStack=(char *)malloc(STACK_SIZE);
    tid=clone(func,childStack+STACK_SIZE,CLONE_VM,&args);
    sleep(5);
    if(CLONE_VM){
        if(y==16){
            printf(1," CLONE_VM  test passed\n",y);
        }
    }
    int x=join(tid);
    if(x==tid)
        printf(1,"Joined Successfully \n");
    sleep(5);
    exit();
}
