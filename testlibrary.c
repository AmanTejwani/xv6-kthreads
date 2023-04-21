#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "thread.h"

int global_var=15;
int vm_func(void *arg){
    if(*((int*)arg)==5){
        (*(int*)arg)++;
        printf(1,"Argument Received Correctly \n");
    }
    else
        printf(1,"Argument Not Received Correctly \n");
    global_var++;
    exit();
}

void vmflag(){
    int arg=5;
    int tid=thread_create(vm_func,&arg);
    if(arg==6){
        printf(1,"CLONE_VM local var test pass  \n");
    }
    else{
        printf(1,"CLONE_VM local var test fail \n");
    }
    if(global_var==16){
        printf(1,"CLONE_VM global var test pass\n");
    }
    else{
        printf(1,"CLONE_VM global var test fail\n");
    }
    thread_join(tid);
}

int tstress_func(void *arg){
    (*(int*)arg)++;
    for(int i=0;i<100;i++);
    exit();
}

void thread_stess_test(){
    int num=20;
    int array[num];
    int x=20;
    for(int i=0;i<num;i++){
        array[i]=thread_create(tstress_func,&x);
    }
    for(int i=0;i<num;i++){
        thread_join(array[i]);
    }
    if(x==40)
        printf(1,"stress test pass \n");
    else
        printf(1,"stress test fail \n");
}

int main(){
    thread_stess_test();
    vmflag();
    exit();
}