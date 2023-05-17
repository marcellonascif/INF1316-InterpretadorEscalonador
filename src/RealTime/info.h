#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

#define SHM_KEY 8000
#define MAX_PROCESSOS 20

struct strProcess{
    int index;
    char processName[10];
    int init;
    char initP; 
    int duration;
    int last;
}; typedef struct strProcess StrProcess;