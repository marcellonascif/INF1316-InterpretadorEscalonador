#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/time.h>

#define EVER ;;

int main(void){

    pid_t pid = getpid();

    printf("Programa 6 - pid: %d\n", pid);
    
    for(EVER){
        printf("Executando P6...\n");
        sleep(1);
    }

    return 0;
}