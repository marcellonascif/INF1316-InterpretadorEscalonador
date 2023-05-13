#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include "processo.h"


#define SHM_KEY 7000
#define MAX_PROCESSOS 20

int main(void)
{

    int i = 0;
    char filename[] = "exec.txt";
    size_t segmento;

    Processo *lstProcessos;
    char nomeProcesso[10];

    segmento = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(Processo), IPC_CREAT | 0666);
    lstProcessos = shmat(segmento, 0, 0);

    FILE *fp = fopen(filename, "r"); // abre o arquivo para leitura
    if (!fp){
        puts("Problem opening file fp");
        exit(1);
    } // Trata problema ao abrir o arquivo

    while (fscanf(fp, "%*s <%[^>]>", nomeProcesso) != EOF){ // lê cada linha do arquivo
        printf("Comando lido: %s\n", nomeProcesso);

        lstProcessos[i].indice = i;
        strcpy(lstProcessos[i].nomeProcesso, nomeProcesso);

        printf("Nome do processo: %s  //  índice: %d\n", lstProcessos[i].nomeProcesso, lstProcessos[i].indice);

        i++;
    }

    fclose(fp); // fecha o arquivo

    return 0;
}