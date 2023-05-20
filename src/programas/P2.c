#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/time.h>

#define EVER ;;
#define SHM_KEY2 7000

int main(int argc, char* argv[]){

    int shmid_pid = shmget(SHM_KEY2, sizeof(pid_t), IPC_CREAT | 0666);
    pid_t* pid = shmat(shmid_pid, 0, 0);
    *pid = getpid();

    // pid_t pidPai = getppid();

    printf("Programa 2 - pid: %d\n", *pid);
    
    for(EVER){
        printf("Executando P2...\n");
        // kill(pidPai, SIGUSR1);
        sleep(1);
    }

    return 0;
}