#include <stdio.h>
#include "processo.h"

int main(void){


    segmento = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(Processo), IPC_CREAT | 0666);
    lstProcessos = shmat(segmento, 0, 0);

    return 0;
}