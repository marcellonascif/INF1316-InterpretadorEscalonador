#include <stdio.h>
#include <stdlib.h>

#include "info.h"

typedef struct queue {
    struct Node* front;
    struct Node* rear;
} Queue;

void initQueue(Queue* q);
int isEmpty(Queue* q);
void enqueue(Queue* q, Process p);
void dequeue(Queue* q);
Process frontData(Queue* q);
void displayQueue(Queue* q);
void queueSort(Queue* q);