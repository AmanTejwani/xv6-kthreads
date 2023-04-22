#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
int main(){
    int x=gettid();
    int y=getpid();
    printf(1,"Running Exec.. CLONE_THREAD test pass and  value of tid is %d and value of pid/tgid is  %d \n",x,y);
    exit();
}