#include <stdlib.h>
#include <stdio.h>

#define SHM_KEY 7000
#define MAX_PROCESSOS 20

enum status {
    PRONTO, PROCESSANDO, ESPERA
}; typedef enum status Status;

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

struct queue {
    Process* ini;
    Process* fim;
    int qtd;  // Quantidade de Processos
};typedef struct queue Queue;
 
Queue *create_queue ();

Process *create_process(char *name, int pid);

Process *copy_process(Process *p);
 
void insert_queue(Queue *f, Process *p);

Process *remove_queue (Queue *f);

void remove_process(Queue *f, Process *p);

void print_queue(Queue *f, char *nameQueue);

void free_process(Process *p);
 
void free_queue(Queue* f);

int empty_queue(Queue * f);