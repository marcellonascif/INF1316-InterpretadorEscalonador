#include <stdio.h>

#define SHM_KEY 7000
#define MAX_PROCESSOS 20

enum status {
    PRONTO, PROCESSANDO, ESPERA
};typedef enum status Status;

struct strProcess{
    int index;
    char processName[10];
}; typedef struct strProcess StrProcess;


struct process {
    char *name; // Nome do Programa
    int pid;
    Status status; // Estado atual do processo
    int execTime;
    int waitTime;
    int io; // 0: Não 1: Sim
    struct process* prox;
}; typedef struct process Process;