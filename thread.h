#define CLONE_VM 1
#define CLONE_FS 2
#define CLONE_FILES 4
#define CLONE_THREAD 8
#define MAX_THREADS 10
#define STACK_SIZE 4096

typedef struct threadLib{
    char *stack;
    char *arg;
    int tid;
    struct threadLib *prev;
    struct threadLib *next;
}threadLib;
struct threadLib *head,*tail;

int thread_create(int (*func)(void *),void *arg);
int thread_join(int tid);
int thread_exit(int tid);
void thread_Add(threadLib *th);
void thread_delete(int tid);