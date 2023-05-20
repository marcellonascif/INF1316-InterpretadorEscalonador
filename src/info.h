#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

#define REAL_TIME 0
#define ROUND_ROBIN 1

#define SHM_KEY 1000
#define SHM_KEY2 7000

#define MAX_PROCESSOS 20

typedef struct process
{
    char name[8]; // Nome do Programa
    int index;     // Numero do processo
    int init;      // Inicio (tempo)
    int duration;  // tempo de duracao
    int policy;    // 0 = REAL_TIME | 1 = ROUND_ROBIN
    int started;   // rodou uma vez ou não
    pid_t pid;
} Process;

typedef struct node
{
    Process process;
    struct node *next;
} Node;

typedef struct queue
{
    Node *front;
    Node *rear;
} Queue;

void initQueue(Queue *q);
int isEmpty(Queue *q);
void enqueue(Queue *q, Process p);
void dequeue(Queue *q);
void displayQueue(Queue *q);
void queueSort(Queue *q);
