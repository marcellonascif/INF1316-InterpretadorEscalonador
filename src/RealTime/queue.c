#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

typedef struct node {
	Process process;
	struct node* next;
} Node;

typedef struct queue {
	Node* front;
	Node* rear;
} Queue;

void initQueue(Queue* q) {
	q->front = NULL;
	q->rear = NULL;
}

int isEmpty(Queue* q) {
	if (q->front == NULL){
		return TRUE;
	}
	return FALSE;
}

void enqueue(Queue* q, Process p) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->process = p;
	newNode->next = NULL;

	if (isEmpty(q)) {
		q->front = newNode;
		q->rear = newNode;
	} else {
		q->rear->next = newNode;
		q->rear = newNode;
	}
}

void dequeue(Queue* q) {
	if (isEmpty(q)) {
		printf("A fila está vazia. Nenhum elemento para remover.\n");
		return;
	}

	Node* temp = q->front;
	q->front = q->front->next; // Segundo da fila

	if (q->front == NULL){ // Se a fila só tinha um elemento
		q->rear = NULL;	   // Fila se torna vazia
	}
		

	free(temp); // Libera o antigo primeiro da fila
}

Process frontData(Queue* q) {
	if (isEmpty(q)) {
		printf("A fila está vazia. Nenhum elemento na frente.\n");
		Process emptyProcess;
		return emptyProcess;
	}

	return q->front->process;
}

void displayQueue(Queue* q) {
	if (isEmpty(q)) {
		printf("A fila está vazia.\n");
		return;
	}

	Node* temp = q->front;
	printf("Elementos da fila: ");

	while (temp != NULL) {
		printf("%d ", temp->process.index);
		temp = temp->next;
	}

	printf("\n");
}

void queueSort(Queue* q) {
	if (isEmpty(q) || q->front->next == NULL) {
		return;
	}

	Node* currNode = q->front;
	int swapped;
	Process temp;

	do {
		swapped = 0;
		currNode = q->front;

		while (currNode->next != NULL) {
			if (currNode->process.init < currNode->next->process.init) {
				temp = currNode->process;
				currNode->process = currNode->next->process;
				currNode->next->process = temp;
				swapped = 1;
			}
			currNode = currNode->next;
		}
	} while (swapped);
}