#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include<conio.h>
#include<ctype.h>
#include "funcoes.h"
#define BUFFER 100

int main(int argc, char *argv[])
{
    gancho *cabeca = criar_lista();
    FILE *arquivo;
    FILE *erro_log;
    int nulo = 0, erro = 0;

    arquivo = fopen(argv[1], "r");
    erro_log = fopen(argv[2], "w");
    
    
    nulo = eh_nulo(arquivo, erro_log);
    if (nulo == -1)
        return -1;
    
    inserir_arquivo(arquivo, cabeca);
    arruma_arquivo(cabeca, erro_log);
    if ((erro = verifica_nomes(cabeca, erro_log)) == -1)
        return -1;
    if ((erro = verifica_args(cabeca, arquivo, erro_log) == -1))
        return -1;

    deleta_lista(cabeca);
    return 0;   
}