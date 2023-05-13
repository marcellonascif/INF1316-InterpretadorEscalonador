#include <stdio.h>

#define SHM_KEY 7000
#define MAX_PROCESSOS 20

struct strProcesso{
    int indice;
    char nomeProcesso[10];
}; typedef struct strProcesso StrProcesso;

struct processo {
    char * nome; // Nome do Programa
    int pid;
    Status status; // Estado atual do processo
    int tempoEmExecucao;
    int tempoEmEspera;
    int IO; // 0: Não 1: Sim
    struct processo * prox;
}; typedef struct processo Processo;