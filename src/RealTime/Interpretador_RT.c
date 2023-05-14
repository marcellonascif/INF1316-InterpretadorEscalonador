#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// #include "info.h"

#define SHM_KEY 8000
#define MAX_PROCESSOS 20

struct strProcess{
    int index;
    char processName[10];
    int init;
    char initP;
    int duration;
}; typedef struct strProcess StrProcess;


int main(void)
{
    int i = 0;
    int inicio = 0;
    int duracao = 0;
	
	char filename[] = "exec.txt";
	size_t segmento;
	StrProcess *lstProcess;
	char processName[10];

	segmento = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(StrProcess), IPC_CREAT | 0666);
    if (segmento == -1) {
    perror("Erro ao alocar memória compartilhada");
    exit(1);
    }
    lstProcess = shmat(segmento, 0, 0);

	FILE *fp = fopen(filename, "r"); // abre o arquivo para leitura
	if (!fp){
		puts("Problem opening file fp");
		exit(1);
	} // Trata problema ao abrir o arquivo
    
    
	while (fscanf(fp, "%*s <%[^>]> I=<%d> D=<%d>", processName, &inicio, &duracao) != EOF){ // lê cada linha do arquivo
        lstProcess[i].init = inicio;
        lstProcess[i].duration = duracao;
		lstProcess[i].index = i;
		strcpy(lstProcess[i].processName, processName);
		printf("Comando lido: Nome do processo: %s  //  índice: %d  //  Início: %d  // Duração: %d\n", lstProcess[i].processName, lstProcess[i].index, lstProcess[i].init, lstProcess[i].duration);

		i++;

		sleep(1);
	}

	fclose(fp); // fecha o arquivo

    // libera a memória compartilhada
	shmctl(segmento, IPC_RMID, 0);
	return 0;
}