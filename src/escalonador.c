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
void handler(int sig);
// void handler1(int sig);
int alocateProcess(Queue* q, Process p);
char* concatenarStrings(const char* str1, const char* str2);
void execProcess(Process p);

//Globais:
// int io_bound = FALSE;
int executing = FALSE;
int termina = FALSE;

int main(void){
	int shmid_process, shmid_pid;
	CurrentProcess* processInfo;
	pid_t* pid;
	pid_t* pidpai;

	struct timeval init, end;
	float sec;
	float secIni;

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
			Queue* filaAux;

			if (processInfo->p.policy == REAL_TIME){
				filaAux = &filaRT;
			}
			else if (processInfo->p.policy == ROUND_ROBIN){
				filaAux = &filaRR;
			}

			processInfo->escalonado = alocateProcess(filaAux, processInfo->p);
			// printf("escalonado = %s\n", (processInfo->escalonado == 0 ? "Não" : "Sim"));

			// puts("Fila RT");
			// displayQueue(&filaRT);
			// puts("Fila RR");
			// displayQueue(&filaRR);
		} 

		// printf("executando = %s\n", (executing == 0 ? "Não" : "Sim"));
		
		/*Inicia a execução dos processos*/ 
		if (executing == TRUE){
			p.pid = *pid;			// pega o pid do processo

			printf("p.name = %s\n", p.name);
			printf("p.pid = %s\n", p.pid);

			/* Espera do REAL TIME */
			if (p.policy == REAL_TIME){
				if(sec == secIni + p.duration){
					// kill(p.pid, SIGSTOP);
					
					dequeue(&filaRT);
					enqueue(&filaRT, p);
					// printf("\n\nFila Real Time:\n");
					// displayQueue(&filaRT); //Imprime Fila de processos Real Time
					executing = FALSE;
				}
			}
			/* Espera do ROUND ROBIN */
			else{
				// kill(p.pid, SIGSTOP);
				dequeue(&filaRR);
				/*if(io_bound == TRUE){
					if(fork() == 0){
						enqueue(&filaIO, p);
						printf("Processo = %s -- PID = %d -- Entrou IO\n", p.name, p.pid);
						sleep(3);
						dequeue(&filaIO);
						io_bound = FALSE;
					}
				}*/
				enqueue(&filaRR, p);
				// printf("\n\nFila Round Robin:\n");
				// displayQueue(&filaRR); //Imprime Fila de processos Round Robin
				executing = FALSE;
			}
			
						
			
		}

		else if (executing == FALSE){
			/* Execução do REAL TIME */
			if ((!isEmpty(&filaRT)) &&  (sec == filaRT.front->process.init)) {  // Primeiro da fila entra em execução
				secIni = sec;
				p = filaRT.front->process;
				if (!p.started){
					execProcess(p); 		// Executa processo pela primeira vez
					// sleep(p.duration);				// deixa o programa parado pelo tempo do processo
					// p.pid = *pid;     		// pega o PID do processo
					p.started = TRUE;		// diz que o processo começou
				}
				else{
					kill(p.pid, SIGCONT);	// Continua o processo já executado uma vez
					// sleep(p.duration);		// deixa o programa parado pelo tempo do processo
				}

				executing = TRUE;
			}

			/* Execução do ROUND ROBIN */
			else if (!isEmpty(&filaRR)){
				p = filaRR.front->process;
				
				if (!p.started){
					execProcess(p);  		// Executa processo pela primeira vez	
					// sleep(1);				// deixa o programa parado pelo tempo do processo
					// p.pid = *pid;     		// pega o PID do processo
					p.started = TRUE;		// diz que o processo começou
				}
				else{
					kill(p.pid, SIGCONT);	// Continua o processo já executado uma vez
					// sleep(p.duration);		// deixa o programa parado pelo tempo do processo
				}	

				executing = TRUE;			
			}
		}
		sleep(1);
	}

	/* Libera a memória compartilhada */ 
	shmctl(shmid_process, IPC_RMID, 0);
	shmctl(shmid_pid, IPC_RMID, 0);
	
	return 0;
}

int alocateProcess(Queue* q, Process p){
	// printf("p.name = %s\n", p.name);
	// printf("p.index = %d\n", p.index);
	// printf("p.init = %d\n", p.init);
	// printf("p.duration = %d\n", p.duration);

	enqueue(q, p);
	queueSort(q);
	
	return TRUE;
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
		// pid_t pid = getpid();
		// pidpai = getppid();
		// printf("pid no escalonador = %d\n", pid);
		printf("Iniciando o programa %s\n", path);
		execvp(path, argv);
	} 
	return;
}
