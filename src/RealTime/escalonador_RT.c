#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

#include "info.h"

int main(void){
    StrProcess * processInfo;
    int shared_memory;
    
    // Obter o identificador da memória compartilhada
    shared_memory = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(StrProcess), IPC_CREAT | 0666);

    // Anexar à memória compartilhada
    processInfo = (StrProcess*)shmat(shared_memory, 0, 0);
    if (processInfo == (StrProcess*)-1) {
        perror("Erro ao anexar à memória compartilhada.\n");
        exit(1);
    }

    int i = 0;

    while(1){
        sleep(1); //espera o interpretador preencher o vetor

        if(processInfo[i].last == -1) {
            break;
        }
        
        printf("\nEscalonador recebendo:\n");
        printf("Nome do processo: %s  //  Índice: %d  //  Início: %d  //  Duração: %d\n", processInfo[i].name, processInfo[i].index, processInfo[i].init, processInfo[i].duration);
        i++;
    }

    // libera a memória compartilhada
	shmctl(shared_memory, IPC_RMID, 0);
    return 0;
}
