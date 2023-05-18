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

int main(void){
	Process * processInfo;
	int shared_memory, sec;
	pid_t pid1, pid2, pid3;
	struct timeval tv;
	
	// Obter o identificador da memória compartilhada
	shared_memory = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(Process), IPC_CREAT | 0666);

	// Anexar à memória compartilhada
	processInfo = (Process*)shmat(shared_memory, 0, 0);
	if (processInfo == (Process*)-1) {
		perror("Erro ao anexar à memória compartilhada.\n");
		exit(1);
	}

	int i = 0;	
	for (EVER) {
		puts("entrei\n");
		sleep(1); //espera o interpretador preencher o vetor

		if(processInfo[i].last == -1) {
			break;
		}
		
		printf("\nEscalonador recebendo:\n");
		printf("Nome do processo: %s  //  Índice: %d  //  Início: %d  //  Duração: %d\n", processInfo[i].name, processInfo[i].index, processInfo[i].init, processInfo[i].duration);
		i++;

		gettimeofday(&tv, NULL);
		sec = tv.tv_sec%60;
		if (sec == 5) {
			kill(SIGSTOP, pid3);
			kill(SIGCONT, pid1);
		}
		if (sec == 25) {
			kill(SIGSTOP, pid1);
			kill(SIGCONT, pid3);
		}
		if (sec == 45) {
			kill(SIGSTOP, pid3);
			kill(SIGCONT, pid2);
		}
		if (sec == 0) {
			kill(SIGSTOP, pid2);
			kill(SIGCONT, pid3);
		}
	}

	// libera a memória compartilhada
	shmctl(shared_memory, IPC_RMID, 0);
	return 0;
}
