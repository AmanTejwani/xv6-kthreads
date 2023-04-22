#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "thread.h"
#define M 3
#define K 2
#define N 3

struct coord{
   int i;
   int j;
};
struct limits {
	int l, h, res;
};

struct ticketLock lk;

int A [M][K] = { {1,4}, {2,5}, {3,6} };
int B [K][N] = { {8,7,6}, {5,4,3} };
int C [M][N];
int Res[M][N]={{28,23,18},{41,34,27},{54,45,36}};
int global_var=15;
int c = 0, c1 = 0, c2 = 0, run = 1;

int firstThread(void *arg){
    while(run == 1){
        ticketLock_acquire(&lk);
        c++;
        ticketLock_release(&lk);
        c1++;
    }
    exit();
}
       
int secondThread(void *arg){
    while(run == 1){
        ticketLock_acquire(&lk);
        c++;
        ticketLock_release(&lk);
        c2++;
    }
    exit();
}

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

int kt_func(void *arg){
    (*(int*)arg)++;
    for(;;);
    exit();
}

int tstress_func(void *arg){
    (*(int*)arg)++;
    for(int i=0;i<100;i++);
    exit();
}

int factorial_func(void *arg) {
	struct limits *p = (struct limits *)arg;
	int prod = 1, k;
	for(k = p->l; k <= p->h; k++)
		prod *= k;
	p->res = prod;
	exit();
}

int matMul_func(void *arg){
    struct coord *data=(struct coord *)arg;
    int n,sum=0;
    for(n=0;n<K;n++){
        sum+=A[data->i][n]*B[n][data->j];
    }
    C[data->i][data->j]=sum;
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

void kill_test(){
    int x=10;
    int y=thread_create(kt_func,&x);
    int ret=thread_exit(y);
    if(ret==-1)
        printf(1,"tkill fail \n");
    else
        printf(1,"tkill pass \n");
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

void factorial_test(){
    int array[2];
    int n=9;
	struct limits lim[2];
    lim[0].l = 1;
	lim[0].h = n/2;
	lim[1].l = n/2 + 1;
	lim[1].h = n;
    array[0]=thread_create(factorial_func,&lim[0]);
    array[1]=thread_create(factorial_func,&lim[1]);
    for(int i=0;i<2;i++){
        thread_join(array[i]);
    }
    int result=lim[0].res * lim[1].res;
    if(result==362880)
        printf(1, "Factorial test passed \n");
    else
        printf(1,"Factorial Test Failed \n");
}

void matrixmultiplication_test(){
    int i,j,count=0;
    for(i = 0; i < M; i++){
        for(j = 0; j < N; j++) {
            struct coord *data = (struct coord *) malloc(sizeof(struct coord));
            data->i = i;
            data->j = j;
            int tid=thread_create(matMul_func,data);
            thread_join(tid);
            count++;
      }
    }
    int flag=1;
    for(int i=0;i<M;i++){
        for(j=0;j<N;j++){
            if(C[i][j]!=Res[i][j]){
                flag=0;
            }
        }
    }
    if(flag)
        printf(1, "Matrix Multiplication test pass \n");
    else
        printf(1, "Matrix Multiplication test fail \n");
}

void sync_test(){
    int array[2];
    initlock(&lk);
    array[0]=thread_create(firstThread,0);
    array[1]=thread_create(secondThread,0);
    run = 0;
    for(int i=0;i<2;i++){
        thread_join(array[i]);
    }
    if(c==c1+c2){
        printf(1, "Ticket Lock Passed\n");
    }
    else{
        printf(1, "Ticket Lock failed\n");
    }
}

int main(){
    kill_test();
    thread_stess_test();
    vmflag();
    factorial_test();
    matrixmultiplication_test();
    sync_test();
    exit();
}