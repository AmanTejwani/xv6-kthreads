#include "types.h"
#include "stat.h"
#include "user.h"
// #include "kalloc.c"

int f(void *arg){
    printf(1," I am in f \n");
    return 1;
}
void clonetest(){
    int tid;
    int args=5;
    // void *stack=(void *)kalloc();
    tid=clone(f,(void *)0,0,(void *)args);
    printf(1,"  value received from clone is %d\n",tid);
}
int main(){
    clonetest();
    exit();
}

