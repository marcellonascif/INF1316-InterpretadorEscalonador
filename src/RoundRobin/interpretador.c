#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "info.h"


#define SHM_KEY 7000
#define MAX_PROCESSOS 20

int main(void)
{

	int i = 0;
	char filename[] = "src/exec.txt";
	size_t segmento;

	StrProcess *lstProcess;
	char processName[10];

	segmento = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(StrProcess), IPC_CREAT | 0666);
	lstProcess = shmat(segmento, 0, 0);

	FILE *fp = fopen(filename, "r"); // abre o arquivo para leitura
	if (!fp){
		puts("Problem opening file fp");
		exit(1);
	} // Trata problema ao abrir o arquivo

	while (fscanf(fp, "%*s <%[^>]>", processName) != EOF){ // lê cada linha do arquivo
		printf("Comando lido: %s\n", processName);

		lstProcess[i].index = i;
		strcpy(lstProcess[i].processName, processName);

		printf("Nome do processo: %s  //  índice: %d\n", lstProcess[i].processName, lstProcess[i].index);

		i++;

		sleep(1);
	}

	fclose(fp); // fecha o arquivo

	return 0;
}