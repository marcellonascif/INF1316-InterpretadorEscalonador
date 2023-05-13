#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#include "info.h"

int main(void){

    size_t segmento;

    StrProcesso *lstProcessos;

    segmento = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(Processo), IPC_CREAT | 0666);
    lstProcessos = shmat(segmento, 0, 0);





    return 0;
}