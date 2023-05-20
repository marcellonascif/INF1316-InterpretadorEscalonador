#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

#include "info.h"

//Prototypes
int isOK(Process *lp, int tam, int inicio, int duracao);

int main(void)
{
	int i = 0;
	int inicio = 0;
	int duracao = 0;
	char policy;

	char filename[] = "exec.txt";
	size_t segmento;
	Process *lstProcess;
	char processName[10];

	segmento = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(Process), IPC_CREAT | 0666);
	if (segmento == -1)
	{
		perror("Erro ao alocar memória compartilhada");
		exit(1);
	}
	lstProcess = shmat(segmento, 0, 0);

	FILE *fp = fopen(filename, "r"); // abre o arquivo para leitura
	if (!fp)
	{
		puts("Problem opening file fp");
		exit(1);
	} // Trata problema ao abrir o arquivo

	pid_t pid = fork();
	if (pid == 0){ // processo filho
		while (fscanf(fp, "%*s <%[^>]> %c=<%d> D=<%d>", processName, &policy, &inicio, &duracao) != EOF){ // lê cada linha do arquivo
			if(policy == 'I'){ // REAL TIME
				if (isOK(lstProcess, i, inicio, duracao)){
					strcpy(lstProcess[i].name, processName);
					lstProcess[i].index = i;
					lstProcess[i].init = inicio;
					lstProcess[i].duration = duracao;
					lstProcess[i].policy = REAL_TIME;
					lstProcess[i].started = FALSE;

					i++;
				}
				
				else{
					printf("Processo: (%s) inválido. Tempo de execução excede o limite permitido.\n", processName);
				}
				
				policy = 'A';
				inicio = -1;
				duracao = 1;
			}
			else{  // ROUND ROBIN
				strcpy(lstProcess[i].name, processName);
				lstProcess[i].index = i;
				lstProcess[i].init = -1;
				lstProcess[i].duration = 1;
				lstProcess[i].policy = ROUND_ROBIN;
				lstProcess[i].started = FALSE;

				i++;
			}
			sleep(1);
		}
	}

	else if (pid > 0){ // processo pai
		char *argv[] = {NULL};
		sleep(1);
		execvp("./escalonador", argv); // executa o escalonador
	}

	fclose(fp); // fecha o arquivo
	return 0;
}

int isOK(Process *lp, int tam, int inicio, int duracao){
	for (int i = 0; i < tam; i++){
		if ((inicio >= lp[i].init) && (inicio <= (lp[i].init + lp[i].duration))){
			return FALSE;
		}

		if (((inicio + duracao) >= lp[i].init) && ((inicio + duracao) <= (lp[i].init + lp[i].duration))){
			return FALSE;
		}
	}

	if ((inicio + duracao) > 60){
		return FALSE;
	}

	return TRUE;
}