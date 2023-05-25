#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/time.h>

#define EVER ;;

int main(void){

    pid_t pid = getpid();

    printf("Programa 3 - pid: %d\n", pid);
    
    for(EVER){
        printf("Executando P3...\n");
        sleep(1);
    }

    return 0;
}