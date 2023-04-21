#define CLONE_VM 1
#define CLONE_FS 2
#define CLONE_FILES 4
#define CLONE_THREAD 8
#define MAX_THREADS 10
#define PGSIZE 4096
struct kthread{
  char *stack;
  void *args;
  int tid;
}kThread;

typedef struct kthread k_Thread;

int thread_create(k_Thread *thread, int (*func)(void *),void *arg);
int thread_join(k_Thread *thread);
