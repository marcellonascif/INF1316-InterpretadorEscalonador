#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>


#define SHM_KEY 8000
#define MAX_PROCESSOS 20

struct strProcess{
    int index;
    char processName[10];
    int init;
    char initP; 
    int duration;
}; typedef struct strProcess StrProcess;



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
    

    for (int i = 0; i < 4; i++) {
        printf("Escalonador recebendo:\n");
        printf("Nome do processo: %s  //  Índice: %d  //  Início: %d  //  Duração: %d\n", processInfo[i].processName, processInfo[i].index, processInfo[i].init, processInfo[i].duration);
    }
    



    // libera a memória compartilhada
	shmctl(shared_memory, IPC_RMID, 0);
    return 0;
}
