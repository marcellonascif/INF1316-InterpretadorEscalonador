#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define EVER ;;
#define SHM_KEY2 5000



int main(void){
    int shmid_pid;
    pid_t *pid;
    shmid_pid = shmget(SHM_KEY2, 20 * sizeof(pid_t), IPC_CREAT | 0666);
    pid = shmat(shmid_pid, 0, 0);
    *pid = getpid();
    printf("Processo 2 - pid: %d\n", *pid);

    for(EVER){
        printf("executando programa 2\n");
        sleep(1);
    }

    return 0;
}