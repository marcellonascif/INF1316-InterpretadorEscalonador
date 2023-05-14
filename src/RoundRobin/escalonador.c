#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "info.h"
//#include "escalonador.h"

//PROTOTYPES
int createProcess (char *program, int index);

int main(void){

    size_t seg;
    StrProcess *lstProcess;

    seg = shmget(SHM_KEY, MAX_PROCESSOS * sizeof(StrProcess), IPC_CREAT | 0666);
    lstProcess = shmat(seg, 0, 0);

    return 0;
}


int createProcess(char *program, int index) {
	char path[] = "../programas/";
	int pid, pidPai;
	char * pidPaiChar;

	pidPai = getpid();
	pid = fork();
	pidPaiChar = converteParaChar(pidPai);

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
		if(signal(SIGUSR1,recebeuSinalIO) == SIG_ERR){
			printf("Processo pai recebeu sinal I/O (Chamando rotina)\n");
		}
	}
	return pid;
}


