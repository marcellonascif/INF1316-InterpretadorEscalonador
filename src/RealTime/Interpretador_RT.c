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

int main(void)
{
    int i = 0;
    int inicio = 0;
    int duracao = 0;
	
	char filename[] = "exec.txt";
	size_t segmento;
	Process *lstProcess;
	char processName[10];

	segmento = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(Process), IPC_CREAT | 0666);
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
    
	pid_t pid = fork();
	if(pid == 0){ // processo filho
		while (fscanf(fp, "%*s <%[^>]> I=<%d> D=<%d>", processName, &inicio, &duracao) != EOF){ // lê cada linha do arquivo
			
			if ((inicio + duracao) < 59){
				strcpy(lstProcess[i].name, processName);
				lstProcess[i].index = i;
				lstProcess[i].init = inicio;
				lstProcess[i].duration = duracao;
				lstProcess[i].last = FALSE;

				i++;
				sleep(1);

				//printf("\nIntepretador:\nComando lido: Nome do processo: %s  //  índice: %d  //  Início: %d  // Duração: %d\n", lstProcess[i].name, lstProcess[i].index, lstProcess[i].init, lstProcess[i].duration);

			}
			else{
				printf("Processo: (%s) inválido. Tempo de execução excede o limite permitido.\n", processName);
			}
			
			/*antiga forma de verificar
			lstProcess[i] = verifyProcess(lstProcess[i]); // verifica se inicio + duracao > 59, se for anula o processo colocando flag de -1 em lstProcesso.inicio*/
		}

		lstProcess[i - 1].last = TRUE; // ultimo a ser lido no arquivo
		
	}
	else if(pid > 0){ // processo pai
		char *argv[] = {NULL};
		// sleep(1);
        execvp("./escalonador_RT", argv); //executa o escalonador
	}

	fclose(fp); // fecha o arquivo
	return 0;
}
