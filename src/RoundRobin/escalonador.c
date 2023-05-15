#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "queue.h"


Process *currentProcess = NULL;
Queue *filaEspera, *filaPronto;

int cpuOciosa; // 0: Não 1: Sim
int tempoCpuOciosa;

#define TEMPO_ESPERA 3 // Tempo limite no estado de espera

//PROTOTYPES
int create_process_pid (char *program, int index);
void ioHandler(int signal);
void atualizaEiniciacurrentProcess();



int create_process_pid(char *program, int index) {
	char path[] = "../programas/";
	int pid, pidPai;
	char *pidPaiChar;

	pidPai = getpid();
	pid = fork();
	pidPaiChar = (char *)pidPai;

	if(pid < 0) { // Erro
		printf("Erro ao criar processo filho.\n");
    		exit(-1);
    	}
	else if (pid == 0) { // Filho 
		char * args[2];
        args[0] = pidPaiChar;
        args[1] = NULL;
		strcat(path, program);  // Programas/programa1 (Exemplo)
        if(execv(path, args) < 0) // Tenta executar o programa
    		printf("Erro ao executar %s.\n", program); 
	} 
	else { // Pai (Pausa o programa que está executando no processo filho)
		kill(pid, SIGSTOP);
		sleep(1);
	    printf("Escalonador: Criou processo filho %s com pid %d\n", program, pid);
		printf("Escalonador: %s recebeu sinal SIGSTOP\n", program);
		if(signal(SIGUSR1,ioHandler) == SIG_ERR){
			printf("Processo pai recebeu sinal I/O (Chamando rotina)\n");
		}
	}
	return pid;
}

void ioHandler(int signal){
	if(signal == SIGUSR1) { 
		currentProcess->io = 1;
	}
}

/*Adiciona processo que recebeu I/O à fila de espera*/
void trataSinalIO(){
	printf("%s enviou sinal SIGUSR1 (I/O)\n", currentProcess->name);
	currentProcess->status = ESPERA;
	insert_queue(filaEspera, copy_process(currentProcess));
	kill(currentProcess->pid, SIGSTOP);
	printf("%s recebeu SIGSTOP e entrou na fila de Espera\n", currentProcess->name);
	print_queue(filaEspera, "Espera");
	if(filaPronto->qtd > 0) atualizaEiniciacurrentProcess();
	else{
		currentProcess = NULL;
		printf("\nCPU está ociosa!\n");
		cpuOciosa = 1;
		tempoCpuOciosa = 0;
	}
}

/*Incrementa 1 u.t. em cada processo em espera*/
void atualizawaitTime() {
	Process *aux;
	aux = filaEspera->ini;

	while(aux != NULL) {
		aux->waitTime++;
		if(aux->waitTime == TEMPO_ESPERA) { // Terminou espera I/O
			aux->status = PRONTO;
			aux->waitTime = 0;
			remove_process(filaEspera, aux);
			printf("%s terminou I/O - Removido da Fila de Espera\n\n", aux->name);
			insert_queue(filaPronto, copy_process(aux));
			print_queue(filaPronto, "Pronto");
		}
		aux = aux->prox;
	}
}

/*Retira processo da fila de prontos e inicia sua execução*/
void atualizaEiniciacurrentProcess(){
	currentProcess = remove_queue(filaPronto);
	if(currentProcess == NULL) {
		printf("Fila Pronto vazia e chamou fila_retira\n");
		 return ;
	}
	kill(currentProcess->pid, SIGCONT);
	currentProcess->status = PROCESSANDO;
	printf("%s recebeu SIGCONT\n", currentProcess->name);
	if(cpuOciosa) { // Terminou I/O
		tempoCpuOciosa = 0;
		cpuOciosa = 0;
	}
	print_queue(filaPronto, "Pronto");
}

void executaEscalonamentoRoundRobin() {
	
	int qtdProcessos, tempoKill = 0;

	printf("\n\n***** Iniciando Escalonamento (Round Robin) *****\n\n");

	print_queue(filaPronto, "Pronto");

	qtdProcessos = filaPronto->qtd;
	atualizaEiniciacurrentProcess();

	// Cada iteração passa 1 u.t.
	while(qtdProcessos > 0){ 

		sleep(1); // Time-Slice de 1 u.t.
		
		if(cpuOciosa) { // Se a CPU estiver ociosa
			tempoCpuOciosa++;
			printf("CPU ociosa por %d u.t.\n", tempoCpuOciosa);
			if(filaEspera->qtd > 0) atualizawaitTime();
		}
		else { // Tem algum processo em execução
			currentProcess->execTime++;
			tempoKill++;
			printf("%s - Tempo de processamento: %d u.t.\n", currentProcess->name, currentProcess->execTime);
			if(filaEspera->qtd > 0) atualizawaitTime();
			if(currentProcess->io) {
				currentProcess->io  = 0;
				trataSinalIO();
			}
		}
	
		// Atualiza processo corrente com próximo processo da Fila Pronto
		if(filaPronto->qtd > 0) {
			if(currentProcess != NULL && currentProcess->status == PROCESSANDO) {
				kill(currentProcess->pid, SIGSTOP);
				printf("\n%s recebeu SIGSTOP\n\n", currentProcess->name);
				currentProcess->status = PRONTO;
				insert_queue(filaPronto, copy_process(currentProcess));
			}
			atualizaEiniciacurrentProcess();
		}
		
		if(tempoKill >= 10) { // Kill no processo corrente
			if(currentProcess != NULL) {
				kill(currentProcess->pid, SIGKILL);
				qtdProcessos--;
				printf("\n%s recebeu SIGKILL - Restam %d processo(s)\n\n", currentProcess->name, qtdProcessos);
				free_process(currentProcess);
				tempoKill = 0;
				if(qtdProcessos == 0) break;
				if(filaPronto->qtd > 0) atualizaEiniciacurrentProcess();
				else cpuOciosa = 0;
			}
		}
		
	}
	printf("\n\n***** Fim do Escalonamento (Round Robin) *****\n\n");
}

int main(void){

    //size_t seg;
    StrProcess *lstProcess;

    // seg = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(StrProcess), IPC_CREAT | 0666);
    lstProcess = shmat(SHM_KEY, 0, 0);

	int i = 0;
	Process *novo;
	
	filaEspera = create_queue();
	filaPronto = create_queue();
	
	while (strlen(lstProcess[i].processName) != 0) {
		int pid = create_process_pid(lstProcess[i].processName, lstProcess[i].index);
		printf("name do programa: %s - pid: %d\n\n",lstProcess[i].processName, pid);
		novo = create_process(lstProcess[i].processName, pid); 
		insert_queue(filaPronto, novo);
		i++;
	}

	executaEscalonamentoRoundRobin();

	// libera a memória compartilhada do processo
	shmdt(lstProcess);

	// libera a memória compartilhada
	//shmctl(segmento, IPC_RMID, 0);
    return 0;
}