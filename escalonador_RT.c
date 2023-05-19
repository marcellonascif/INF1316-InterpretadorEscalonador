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

int main(void)
{
    Process currentP;
    Process *processInfo;

    int shared_memory;
    int i, j;
    float sec;

    pid_t pid1;

    struct timeval init, end;

    // Obter o identificador da memória compartilhada
    shared_memory = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(Process), IPC_CREAT | 0666);

    // Anexar à memória compartilhada
    processInfo = (Process *)shmat(shared_memory, 0, 0);
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
        currentP = processInfo[i];
        printf("\n%s\n", currentP.name);

        if (sec == currentP.init)
        {
            puts("Entrei\n");
            kill(SIGCONT, pid1);
            sleep(currentP.duration);
            kill(SIGSTOP, pid1);
        }
        /* Processo do RoundRobin
        else{
            kill(SIGCONT, pid1);
            sleep(1);
            kill(SIGSTOP, pid1);
        }*/

        if (currentP.last != TRUE)
        {
            // printf("%s\n", currentP.name);
            printf("currentP.init = %d\n", currentP.init);
            printf("currentP.duration = %d\n", currentP.duration);
            printf("currentP.last = %d\n\n", currentP.last);

            enqueue(&filaRT, currentP);
            queueSort(&filaRT);

            i++;
        }

        else
        {
            puts("ultima vez");
            printf("currentP.init = %d\n", currentP.init);
            printf("currentP.duration = %d\n", currentP.duration);
            printf("currentP.last = %d\n\n", currentP.last);
            // printf("sec = %d\n\n", sec);
            enqueue(&filaRT, currentP);
            queueSort(&filaRT);
            break;
        }
    }
    displayQueue(&filaRT);

    // libera a memória compartilhada
    shmctl(shared_memory, IPC_RMID, 0);
    return 0;
}