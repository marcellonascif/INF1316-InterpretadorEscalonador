#include <stdlib.h>
#include <stdio.h>
#include <info.h>

enum status {
	PRONTO, PROCESSANDO, ESPERA
};

typedef enum status Status;


struct queue {
    Process* ini;
    Process* fim;
    int qtd;  // Quantidade de Processos
};

typedef struct queue Queue;
 
Queue* create_queue ();

Process * create_process(char * nome, int pid);

Process * copy_process(Process *p);
 
void insert_queue(Queue* f, Process *p);

Process * remove_queue (Queue* f);

void remove_process(Queue *f, Process *p);

void print_queue(Queue *f, char *nomeQueue);

void free_process(Process *p);
 
void free_queue(Queue* f);

int empty_queue(Queue * f);