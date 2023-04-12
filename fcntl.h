#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_CREATE  0x200
#define SEEK_SET 0 // for lseek syscall from Starting offset
#define SEEK_CUR 1 // for lseek syscall from current offset 
#define SEEK_END 2 // for lseek syscall from ending offset