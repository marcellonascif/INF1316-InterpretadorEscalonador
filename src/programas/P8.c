#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/time.h>

#define EVER ;;
#define SHM_KEY2 7000



int main(void){

    int shmid_pid = shmget(SHM_KEY2, sizeof(pid_t), IPC_CREAT | 0666);
    pid_t* pid = shmat(shmid_pid, 0, 0);

    *pid = getpid();

    printf("Programa 8 - pid: %d\n", *pid);
    
    for(EVER){
        printf("Executando P8...\n");
        sleep(1);
    }

    return 0;
}