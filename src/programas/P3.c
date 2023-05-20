#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/time.h>

#define EVER ;;
#define SHM_KEY2 7000



int main(void){
    struct timeval init, end;
    float sec;

    int shmid_pid = shmget(SHM_KEY2, sizeof(pid_t), IPC_CREAT | 0666);
    pid_t* pid = shmat(shmid_pid, 0, 0);

    gettimeofday(&init, NULL);

    *pid = getpid();

    puts("P3");
    
    for(EVER){
        gettimeofday(&end, NULL);
        sec = ((end.tv_sec - init.tv_sec) % 60) + 1;
        printf("%.1f s\n", sec);
        sleep(1);
    }

    return 0;
}