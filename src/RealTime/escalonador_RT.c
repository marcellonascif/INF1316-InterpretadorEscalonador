#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
// #include "queue.h"
#include "info.h"

#define SHM_KEY2 5000
//Protótipos:
char* concatenarStrings(const char* str1, const char* str2);
void execProcess(Process currentP);

int main(void)
{
    Process currentP;
    Process *processInfo;

    int shared_memory, shmid_pid, i;
    float sec;
    char *argv[] = {NULL};
    pid_t *ptr_pid;


    struct timeval init, end;

    // Obter o identificador da memória compartilhada
    shared_memory = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(Process), IPC_CREAT | 0666);
    shmid_pid = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(pid_t), IPC_CREAT | 0666);

    // Anexar à memória compartilhada
    processInfo = (Process *)shmat(shared_memory, 0, 0);
    ptr_pid = (pid_t*)shmat(shmid_pid, 0, 0);

    if (processInfo == (Process *)-1)
    {
        perror("Erro ao anexar à memória compartilhada.\n");
        exit(1);
    }

    Queue filaRR;
    Queue filaRT;
    Queue filaIO;

    initQueue(&filaRR);
    initQueue(&filaRT);
    initQueue(&filaIO);

    i = 0;
    gettimeofday(&init, NULL);

    for (EVER)
    {
        gettimeofday(&end, NULL);
        sec = ((end.tv_sec - init.tv_sec) % 60);
        sleep(1); // espera o interpretador preencher o vetor
        printf("tempo: %.1f s\n", sec);
        if(currentP.last != -1){
            currentP = processInfo[i];
            printf("\n%s\n", currentP.name);
        } 
        
        if(currentP.last == FALSE){ // Se o processo não for o ultimo entra aqui
            printf("currentP.init = %d\n", currentP.init);
            printf("currentP.duration = %d\n", currentP.duration);
            printf("currentP.last = %d\n\n", currentP.last);
            enqueue(&filaRT, currentP);
            queueSort(&filaRT);
            // printf("**************\nINICIO DO PRIMEIRO DA FILA:%d\n**************\n", filaRT.front->process.init);
            // displayQueue(&filaRT); //Imprime Fila de processos Real-Time
            i++;
        }
        // currentP.pid = *pid; 

        else if(currentP.last == TRUE){ // Se o processo for o último entra aqui
            printf("\n%s\n", currentP.name);
            printf("currentP.init = %d\n", currentP.init);
            printf("currentP.duration = %d\n", currentP.duration);
            printf("currentP.last = %d\n\n", currentP.last);
            enqueue(&filaRT, currentP);
            queueSort(&filaRT);
            displayQueue(&filaRT); //Imprime Fila de processos Real-Time
            currentP.last = -1;
        }

        if (sec == filaRT.front->process.init)  // Primeiro da fila entra em execução
        {   
            execProcess(filaRT.front->process); // Executa processo e armazena seu pid em currentP.pid
            filaRT.front->process.pid = *ptr_pid;
            printf("***************\n%s pid lido pelo escalonador: %d\n******************\n", filaRT.front->process.name, filaRT.front->process.pid);
            kill(SIGCONT, filaRT.front->process.pid);
            sleep(filaRT.front->process.duration);
            kill(SIGSTOP, filaRT.front->process.pid);
            dequeue(&filaRT);
            enqueue(&filaRT, currentP);
        }
        /* Processo do RoundRobin
        else{
            kill(SIGCONT, pid1);
            sleep(1);
            kill(SIGSTOP, pid1);
        }*/
        
    }

    // libera a memória compartilhada
    shmctl(shared_memory, IPC_RMID, 0);
    shmctl(shmid_pid, IPC_RMID, 0);
    return 0;
}


char* concatenarStrings(const char* str1, const char* str2) {
    size_t tamanhoStr1 = strlen(str1);
    size_t tamanhoStr2 = strlen(str2);
    size_t tamanhoTotal = tamanhoStr1 + tamanhoStr2 + 1;

    char* resultado = (char*)malloc(tamanhoTotal);

    if (resultado == NULL) {
        perror("Erro ao alocar memória");
        exit(1);
    }

    strcpy(resultado, str1);
    strcat(resultado, str2);

    return resultado;
}

void execProcess(Process currentP){
    char inicioPath[] = "../programas/";
    char *path;
    char *argv[] = {NULL};

    path = concatenarStrings(inicioPath, currentP.name);
    
    if(fork() == 0){
        printf("Executando o %s\n", path);
         execvp(path, argv);
    } 
    return;
}
