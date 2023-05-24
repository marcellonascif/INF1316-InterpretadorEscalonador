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
	char filename[] = "exec.txt";
	FILE *fp = fopen(filename, "r"); // abre o arquivo para leitura
	if (!fp)
	{
		puts("Problem opening file fp");
		exit(1);
	} // Trata problema ao abrir o arquivo

	char processName[10];
	int i = 0;
	int inicio = 0;
	int duracao = 0;
	char policy;

	Process lstProcess[MAX_PROCESSOS];

	size_t segmento = shmget(SHM_KEY, sizeof(CurrentProcess), IPC_CREAT | 0666);
	if (segmento == -1)
	{
		perror("Erro ao alocar memória compartilhada");
		exit(1);
	}
	CurrentProcess* process = shmat(segmento, 0, 0);

	pid_t pid = fork();
	if (pid == 0){ /* Processo Filho */
		while (fscanf(fp, "%*s <%[^>]> %c=<%d> D=<%d>", processName, &policy, &inicio, &duracao) != EOF){ // lê cada linha do arquivo
			if(policy == 'I'){ /* REAL TIME */
				if (isOK(lstProcess, i, inicio, duracao)){

					strcpy(process->p.name, processName);
					process->p.index = i;
					process->p.init = inicio;
					process->p.duration = duracao;
					process->p.policy = REAL_TIME;
					process->p.started = FALSE;
					process->escalonado = FALSE;

					lstProcess[i] = process->p;
					i++;
				}
				
				else{
					printf("Processo: (%s) inválido. Tempo de execução excede o limite permitido.\n", processName);
				}
				
				policy = 'A';
				inicio = -1;
				duracao = 1;
			}
			else{  /* ROUND ROBIN */
				strcpy(process->p.name, processName);
				process->p.index = i;
				process->p.init = -1;
				process->p.duration = 1;
				process->p.policy = ROUND_ROBIN;
				process->p.started = FALSE;
				process->escalonado = FALSE;

				lstProcess[i] = process->p;
				i++;
			}
			sleep(1);
		}
	}

	else if (pid > 0){ /* Processo Pai */
		char *argv[] = {NULL};
		// sleep(1);
		execvp("./escalonador", argv); /* Executa o escalonador */
	}

	fclose(fp); /* Fecha o arquivo */
	return 0;
}

int isOK(Process *lp, int tam, int inicio, int duracao){
	for (int i = 0; i < tam; i++){
		if ((inicio >= lp[i].init) && (inicio < (lp[i].init + lp[i].duration))){
			return FALSE;
		}

		if (((inicio + duracao) >= lp[i].init) && ((inicio + duracao) <= (lp[i].init + lp[i].duration))){
			return FALSE;
		}
	}

	if ((inicio + duracao) > 59){
		return FALSE;
	}

	return TRUE;
}