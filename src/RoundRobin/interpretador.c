#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "queue.h"

int main(void)
{

	int i = 0;
	char filename[] = "exec.txt";
	char processName[10];

	size_t segmento = shmget(SHM_KEY1, MAX_PROCESSOS * sizeof(StrProcess), IPC_CREAT | 0666);
	StrProcess *lstProcess = shmat(segmento, 0, 0);

	size_t segmento2 = shmget(SHM_KEY2, sizeof(int), IPC_CREAT | 0666);
	int *lastId = shmat(segmento2, 0, 0);

	FILE *fp = fopen(filename, "r"); // abre o arquivo para leitura
	if (!fp){
		puts("Problem opening file fp");
		exit(1);
	} // Trata problema ao abrir o arquivo

	while (fscanf(fp, "%*s <%[^>]>", processName) != EOF){ // lê cada linha do arquivo
		printf("Comando lido: %s\n", processName);

		lstProcess[i].index = i;
		strcpy(lstProcess[i].name, processName);

		printf("Nome do processo: %s  //  índice: %d\n", lstProcess[i].name, lstProcess[i].index);

		i++;

		sleep(1);
	}

	// tentativa de passar a qtd de processos na memória compartilhada
	*lastId = i;

	for (int i = 0; i < *lastId; i++){
		printf("processo = %s\n", lstProcess[i].name);
	}

	fclose(fp); // fecha o arquivo

	return 0;
}