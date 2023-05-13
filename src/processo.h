#include <stdio.h>

#define SHM_KEY 7000
#define MAX_PROCESSOS 20

struct processo{
    int indice;
    char nomeProcesso[10];
}; typedef struct processo Processo;