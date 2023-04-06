#include "types.h"
#include "stat.h"
#include "user.h"
#define CLONE_VM 1
int f(void *arg){
    printf(1," I am in f \n");
    return 1;
}
void clonetest(){
    int tid;
    int args=5;
    tid = clone(f, (void *)0, CLONE_VM, (void *)args); 
    printf(1,"  value received from clone is %d\n",tid);
}
int main(){
    clonetest();
    exit();
}