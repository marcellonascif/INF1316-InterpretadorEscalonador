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
	Process* processInfo;
	Process currentP;
	int shared_memory, sec;
	int i, j;
	pid_t pid1;
	struct timeval t1;
	
	// Obter o identificador da memória compartilhada
	shared_memory = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(Process), IPC_CREAT | 0666);

	// Anexar à memória compartilhada
	processInfo = (Process*)shmat(shared_memory, 0, 0);
	if (processInfo == (Process*)-1) {
		perror("Erro ao anexar à memória compartilhada.\n");
		exit(1);
	}

	i = 0;	
	for (EVER) {
		sleep(1); //espera o interpretador preencher o vetor

		currentP = processInfo[0];
		printf("%s\n", currentP.name);

		// printf("\nEscalonador recebendo:\n");
		// printf("Nome do processo: %s  //  Índice: %d  //  Início: %d  //  Duração: %d\n", processInfo[i].name, processInfo[i].index, processInfo[i].init, processInfo[i].duration);

		//realTime(Queue* pronto);
		

		gettimeofday(&t1, NULL);
		sec = (t1.tv_sec % 60);

		if (sec == currentP.init){
			puts("Entrei");
			kill(SIGCONT, pid1);
			sleep(currentP.duration);
			kill(SIGSTOP, pid1);

			currentP.duration = 0;
		}


		if (currentP.last != TRUE){
			//printf("%s\n", currentP.name);
			printf("currentP.init = %d\n", currentP.init);
			printf("currentP.duration = %d\n", currentP.duration);
			printf("currentP.last = %d\n\n", currentP.last);

			i++;
        }
			
		else{
			puts("ultima vez");
			printf("sec = %d\n\n", sec);
		}
	}

	for (int z = 0; z < i; z++){
		for (j = z + 1; j < i; j++) {
			if (processInfo[j].init < processInfo[z].init){
				Process temp = processInfo[z];
				processInfo[z] = processInfo[j];
				processInfo[j] = temp;
			}
		}
	}
	
	for (int j = 0; j < i; j++) {
		printf("processInfo[%d] = %s\n", j, processInfo[j].name);
	}

	// libera a memória compartilhada
	shmctl(shared_memory, IPC_RMID, 0);
	return 0;
}
