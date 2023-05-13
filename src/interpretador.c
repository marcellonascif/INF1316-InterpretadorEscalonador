#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define MAX_PROCESSOS 20

struct processo{
   int indice;
   char nomeProcesso[10];
}; typedef struct processo Processo;

int main(void){
   FILE *fp;
   char filename[] = "exec.txt";
   size_t len = 0;
   ssize_t read = 0;
   int i = 0;
   Processo* listaProcessos;
   char nomeProcesso[10];

   listaProcessos = (Processo*) malloc(MAX_PROCESSOS * sizeof(Processo));

   fp = fopen(filename, "r"); // abre o arquivo para leitura

   if (fp == NULL) { // verifica se houve erro na abertura do arquivo
      printf("Erro ao abrir o arquivo.\n");
      return 1;
   }

   while (fscanf(fp, "%*s %s", nomeProcesso) != EOF) { // lê cada linha do arquivo
      printf("Comando lido: %s\n", nomeProcesso);
      
      listaProcessos[i].indice = i;
      strcpy(listaProcessos[i].nomeProcesso, nomeProcesso);

      printf("Nome do processo: %s  //  índice: %d\n", listaProcessos[i].nomeProcesso, listaProcessos[i].indice);

      i++;
   }

   fclose(fp); // fecha o arquivo

   return 0;
}