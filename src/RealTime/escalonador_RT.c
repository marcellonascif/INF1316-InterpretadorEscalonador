#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>

#include "info.h"

//Protótipos:
char* concatenarStrings(const char* str1, const char* str2);
void execProcess(Process currentP);

int main(void){
	int shared_memory, shmid_pid;
	Process currentP;
	Process *processInfo;
	pid_t* pid;
	
	int i = 0;
	
	struct timeval init, end;
	float sec;

	// Anexar à memória compartilhada
	shared_memory = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(Process), IPC_CREAT | 0666);
	processInfo = (Process *)shmat(shared_memory, 0, 0);
	if (!processInfo)
	{
		perror("Erro ao anexar à memória compartilhada.\n");
		exit(1);
	}

	// Anexar à memória compartilhada
	shmid_pid = shmget(SHM_KEY2, sizeof(pid_t), IPC_CREAT | 0666);
	pid = shmat(shmid_pid, 0, 0);

	Queue filaRR;
	Queue filaRT;
	Queue filaIO;

	initQueue(&filaRR);
	initQueue(&filaRT);
	initQueue(&filaIO);

	gettimeofday(&init, NULL);

	for (EVER)
	{
		gettimeofday(&end, NULL);
		sec = ((end.tv_sec - init.tv_sec) % 60);
		sleep(1); // espera o interpretador preencher o vetor
		printf("tempo: %.1f s\n", sec);
		
		if(processInfo[i].index == i){ // Se o processo não for o ultimo entra aqui
			currentP = processInfo[i];

			printf("currentP.name = %s\n", currentP.name);
			printf("currentP.init = %d\n", currentP.init);
			printf("currentP.duration = %d\n", currentP.duration);

			enqueue(&filaRT, currentP);
			queueSort(&filaRT);
			//displayQueue(&filaRT); //Imprime Fila de processos Real-Time

			printf("currentP.index = %d\n", currentP.index);
			printf("i = %d\n\n", i);
			i++;
		}

		if (filaRT.front->process.init == sec){  // Primeiro da fila entra em execução
		   
			Process p = filaRT.front->process;
			execProcess(p); // Executa processo e armazena seu pid em currentP.pid
			sleep(p.duration);
			//kill(p.pid, SIGCONT);

			p.pid = *pid;
			printf("***************\n%s pid lido pelo escalonador: %d\n******************\n", filaRT.front->process.name, p.pid);
			
			kill(p.pid, SIGSTOP);
			
			dequeue(&filaRT);
			enqueue(&filaRT, p);
			displayQueue(&filaRT); //Imprime Fila de processos Real-Time

		}
		/* Processo do RoundRobin 
		else{

			sleep(1);
			kill(SIGSTOP, pid1);
		}*/
	}

	// libera a memória compartilhada
	shmctl(shared_memory, IPC_RMID, 0);
	shmctl(shmid_pid, IPC_RMID, 0);
	
	return 0;
}


char* concatenarStrings(const char* str1, const char* str2) {
	size_t tamanhoStr1 = strlen(str1);
	size_t tamanhoStr2 = strlen(str2);
	size_t tamanhoTotal = tamanhoStr1 + tamanhoStr2 + 1;

	char* resultado = (char*)malloc(tamanhoTotal);

	if (resultado == NULL) {
		perror("Erro ao alocar memória");
		exit(1);
	}

	strcpy(resultado, str1);
	strcat(resultado, str2);

	return resultado;
}

void execProcess(Process p){
	char inicioPath[] = "../programas/";
	char *path;
	char *argv[] = {NULL};

	path = concatenarStrings(inicioPath, p.name);
	
	if(fork() == 0){
		printf("Executando o %s\n", path);
		execvp(path, argv);
	} 
	return;
}
