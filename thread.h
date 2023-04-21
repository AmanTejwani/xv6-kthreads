#define CLONE_VM 1
#define CLONE_FS 2
#define CLONE_FILES 4
#define CLONE_THREAD 8
#define MAX_THREADS 10
#define STACK_SIZE 4096

int thread_create(int (*func)(void *),void *arg);
int thread_join(int tid);
