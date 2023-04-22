#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "thread.h"
#define M 3
#define K 2
#define N 3

struct thread_data {
    int *arr;
    int low;
    int high;
    int max_sum;
};

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
char *argv[] = { "clonethread", 0 };

int find_maximum_subarray(void *arg) {
    struct thread_data *data = (struct thread_data *) arg;
    int *arr = data->arr;
    int low = data->low;
    int high = data->high;
    int max_sum = data->max_sum;

    if (low == high) {
        if (arr[low] > 0) {
            max_sum = arr[low];
        } else {
            max_sum = 0;
        }
    } else {
        int mid = (low + high) / 2;
        int left_max_sum = 0;
        int left_sum = 0;
        for (int i = mid; i >= low; i--) {
            left_sum += arr[i];
            if (left_sum > left_max_sum) {
                left_max_sum = left_sum;
            }
        }
        int right_max_sum = 0;
        int right_sum = 0;
        for (int i = mid + 1; i <= high; i++) {
            right_sum += arr[i];
            if (right_sum > right_max_sum) {
                right_max_sum = right_sum;
            }
        }
        int sum = left_max_sum + right_max_sum;
        if (sum > max_sum) {
            max_sum = sum;
        }
        struct thread_data left_data = { arr, low, mid, max_sum };
        struct thread_data right_data = { arr, mid + 1, high, max_sum };
        int array[2];
        if (data->max_sum < max_sum) {
            array[0] = thread_create(find_maximum_subarray, (void *) &left_data);
            if(array[0] == -1) {
                printf(1,"Error: unable to create thread1, %d\n", array[0]);
                exit();
            }
            array[1] = thread_create(find_maximum_subarray, (void *) &right_data);
            if(array[1] == -1) {
                printf(1,"Error: unable to create thread2, %d\n", array[1]);
                exit();
            }
            thread_join(array[0]);
            thread_join(array[1]);
        } else {
            find_maximum_subarray((void *) &left_data);
            find_maximum_subarray((void *) &right_data);
        }
    }
    data->max_sum = max_sum;
    exit();
}

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

int func_CLONE_FILES(void *arg){
    int fd = *(int *)arg;
    lseek(fd, 5, SEEK_CUR);
    exit();
}

int func_args(void *args)
{
    struct coord *var = (struct coord *)args;
    if((var->i*var->j)==6)
        printf(1,"Multi Arg test pass \n");
    else
        printf(1,"Multi Arg test fail \n");
    exit();
}

int bar(void *arg){
    exec("clonethread", argv);
    int x=gettid();
    int y=getpid();
    printf(1,"In Bar value of tid is %d and value of pid/tgid is  %d \n",x,y);
    exit();
}
int foo(void *arg){
    int args=6;
    int tid=thread_create(bar,&args);
    thread_join(tid);
    exit();
}

int fork_func(void *arg){
    int pid=fork();
    if(pid<0)
        printf(1,"Fork test failed \n");
    if(pid==0){
        char *argv[]={"echo","Fork test passed"};
        exec("echo",argv);
    }else{
        wait();
    }
    exit();
}

void vmflag(){
    int arg=5;
    int tid=thread_create(vm_func,&arg);
    if(tid == -1) {
        printf(1,"Error: unable to create thread1, %d\n", tid);
        exit();
    }
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
    if(y == -1) {
        printf(1,"Error: unable to create thread1, %d\n", y);
        exit();
    }
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
        if(array[i] == -1) {
            printf(1,"Error: unable to create thread1, %d\n", array[i]);
            exit();
        }
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
    if(array[0] == -1) {
        printf(1,"Error: unable to create thread1, %d\n", array[0]);
        exit();
    }
    array[1]=thread_create(factorial_func,&lim[1]);
    if(array[1] == -1) {
        printf(1,"Error: unable to create thread2, %d\n", array[1]);
        exit();
    }
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
            if(tid == -1) {
                printf(1,"Error: unable to create thread1, %d\n", tid);
                exit();
            }
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
    if(array[0] == -1) {
        printf(1,"Error: unable to create thread1, %d\n", array[0]);
        exit();
    }
    array[1]=thread_create(secondThread,0);
    if(array[1] == -1) {
        printf(1,"Error: unable to create thread1, %d\n", array[1]);
        exit();
    }
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

void maximum_subarray_sum_test(){
    int arr[] = { -2, -5, 6, -2, -3, 1, 5, -6 };
    int n = sizeof(arr) / sizeof(arr[0]);
    struct thread_data data = { arr, 0, n - 1, 0 };
    int tid;
    tid = thread_create(find_maximum_subarray,&data);
    if(tid == -1) {
        printf(1,"Error: unable to create thread1, %d\n", tid);
        exit();
    }
    thread_join(tid);
    if(data.max_sum == 7){
        printf(1, "Maximum Subarray Sum test passed\n");
    }
    else{
        printf(1, "Maximum Subarray Sum test failed\n");
    }
}


void mutiple_Arg(){
    struct coord *tmp = (struct coord *)malloc(sizeof(struct coord));
    tmp->i = 2;
    tmp->j = 3;
    int tid=thread_create(func_args,tmp);
    thread_join(tid);
}

void cloneFile(){
    int fd = open("test.txt",O_CREATE | O_RDWR);
    int tid=thread_create(func_CLONE_FILES,&fd);
    int z = lseek(fd,0,SEEK_CUR);
    if(z == 5)
        printf(1, "CLONE_FILES passed\n");
    else
        printf(1, "CLONE_FILES failed\n");
    thread_join(tid);
}

void clonethread(){
    int args=5;
    int tid=thread_create(foo,&args);
    thread_join(tid);
}

void forkTest(){
    int tid=thread_create(fork_func,0);
    thread_join(tid);
}
int main(){
    printf(1,"........ Clone Test's Starting ........\n");
    forkTest();
    cloneFile();
    mutiple_Arg();
    kill_test();
    thread_stess_test();
    vmflag();
    factorial_test();
    matrixmultiplication_test();
    maximum_subarray_sum_test();
    sync_test();
    clonethread();
    exit();
}