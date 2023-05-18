#include <stdio.h>
#include <stdlib.h>

#define SHM_KEY 8000
#define MAX_PROCESSOS 20
#define EVER ;;

typedef struct process{
    char name[10]; // Nome do Programa
    int index;     // Numero do processo
    int init;      // Inicio (tempo)
    int duration;  // tempo de duracao
    int last;      // é o último (0 = false | 1 = true)
} Process;

/*typedef struct process {
    char name[10]; // Nome do Programa
    int pid;
    Status status; // Estado atual do processo
    int execTime;
    int waitTime;
    int io; // 0: Não 1: Sim
    struct process* prox;
} Process;*/