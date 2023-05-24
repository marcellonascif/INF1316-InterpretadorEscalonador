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
// void handler1(int sig);
void handler(int sig);
char* concatenarStrings(const char* str1, const char* str2);
void execProcess(Process currentP);

//Globais:
// int io_bound = FALSE;
int termina = FALSE;

int main(void){
	int shmid_process, shmid_pid;
	CurrentProcess* processInfo;
	pid_t* pid;

	Process currentP;
	
	struct timeval init, end;
	float sec;

	// Anexar à memória compartilhada do processo recebido do interpretador
	shmid_pid = shmget(SHM_KEY2, sizeof(pid_t), IPC_CREAT | 0666);
	if (!shmid_pid){ perror("Erro ao criar a memória compartilhada do shmid_pid.\n"); exit(1);}
	pid = shmat(shmid_pid, 0, 0);

	// Anexar à memória compartilhada do pid recebido pelo processo executado na fila
	shmid_process = shmget(SHM_KEY, sizeof(CurrentProcess), IPC_CREAT | 0666);
	if (!shmid_process){ perror("Erro ao criar a memória compartilhada do shmid_process.\n"); exit(1);}
	processInfo = (CurrentProcess*)shmat(shmid_process, 0, 0);
	
	Queue filaRR;
	Queue filaRT;
	Queue filaIO;
	initQueue(&filaRR);
	initQueue(&filaRT);
	initQueue(&filaIO);

	Process p;

	// signal(SIGUSR1, handler1); // recebeu sinal de IO bound
	signal(SIGINT, handler); // Se receber interrupção do teclado (Ctrl+C), QUEBRA O LOOP INFINITO

	gettimeofday(&init, NULL);
	while (!termina){

		gettimeofday(&end, NULL);
		sec = ((end.tv_sec - init.tv_sec) % 60);
		printf("\n%.1f'\n", sec);
		
		if (processInfo->escalonado == FALSE){/*Se ainda recebe processo entra aqui*/ 
			currentP = processInfo->p;

			printf("currentP.name = %s\n", currentP.name);
			printf("currentP.index = %d\n", currentP.index);
			printf("currentP.init = %d\n", currentP.init);
			printf("currentP.duration = %d\n", currentP.duration);
			printf("escalonado = %s\n", (processInfo->escalonado == 0 ? "Não" : "Sim"));

			if (currentP.policy == REAL_TIME){
				enqueue(&filaRT, currentP);
				queueSort(&filaRT);
				puts("Fila RT");
				displayQueue(&filaRT);
			}

			else if (currentP.policy == ROUND_ROBIN){
				enqueue(&filaRR, currentP);
				queueSort(&filaRR);
				puts("Fila RR");
				displayQueue(&filaRR);

			}
			processInfo->escalonado = TRUE;
		} 


		/*Inicia a execução dos processos*/ 
		/* Processo do Real Time */
		// if ((!isEmpty(&filaRT)) && (filaRT.front->process.init == sec)) {  // Primeiro da fila entra em execução
		   
		// 	p = filaRT.front->process;
		// 	if (!p.started){
		// 		execProcess(p); 		// Executa processo pela primeira vez
		// 		sleep(p.duration);		// deixa o programa parado pelo tempo do processo
		// 		p.pid = *pid;			// pega o pid do processo
		// 		p.started = TRUE;		// diz que o processo começou
		// 	}
		// 	else{
		// 		kill(p.pid, SIGCONT);	// Continua o processo já executado uma vez
		// 		sleep(p.duration);		// deixa o programa parado pelo tempo do processo
		// 	}

		// 	kill(p.pid, SIGSTOP);
			
		// 	dequeue(&filaRT);
		// 	enqueue(&filaRT, p);
		// 	printf("\n\nFila Real Time:\n");
		// 	displayQueue(&filaRT); //Imprime Fila de processos Real Time

		// }
		// /* Processo do Round Robin */
		// else if (!isEmpty(&filaRR)){
		// 	p = filaRR.front->process;
			
		// 	if (!p.started){
		// 		execProcess(p);  		// Executa processo pela primeira vez	
		// 		sleep(p.duration);		// deixa o programa parado pelo tempo do processo
		// 		p.pid = *pid;     		// pega o PID do processo
		// 		p.started = TRUE;		// diz que o processo começou
		// 	}
		// 	else{
		// 		kill(p.pid, SIGCONT);	// Continua o processo já executado uma vez
		// 		sleep(p.duration);		// deixa o programa parado pelo tempo do processo
		// 	}

		// 	kill(p.pid, SIGSTOP);
						
		// 	dequeue(&filaRR);
        //     /*if(io_bound == TRUE){
        //         if(fork() == 0){
        //             enqueue(&filaIO, p);
		// 			printf("Processo = %s -- PID = %d -- Entrou IO\n", p.name, p.pid);
        //             sleep(3);
        //             dequeue(&filaIO);
        //             io_bound = FALSE;
        //         }
        //     }*/
		// 	enqueue(&filaRR, p);
		// 	printf("\n\nFila Round Robin:\n");
		// 	displayQueue(&filaRR); //Imprime Fila de processos Round Robin
		// }
		sleep(1);

	}

	/* Libera a memória compartilhada */ 
	shmctl(shmid_process, IPC_RMID, 0);
	shmctl(shmid_pid, IPC_RMID, 0);
	
	return 0;
}

void handler(int sig) {
	termina = TRUE;
}

/*void handler1(int sig) {
	io_bound = TRUE;
}*/

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
	char inicioPath[] = "./programas/";
	char *path;

	path = concatenarStrings(inicioPath, p.name);
	
	char *argv[] = {NULL};
	
	if(fork() == 0){
		printf("Iniciando o programa %s\n", path);
		execvp(path, argv);
	} 
	return;
}
