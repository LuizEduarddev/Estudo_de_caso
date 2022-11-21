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
    FILE *arquivo_pronto;
    int nulo = 0, erro = 0;

    arquivo = fopen(argv[1], "r");
    arquivo_pronto = fopen(argv[2], "w");
    erro_log = fopen(argv[3], "w");

    nulo = eh_nulo(arquivo, arquivo_pronto, erro_log);
    if (nulo == -1)
    {
        return -1;
    }
    inserir_arquivo(arquivo, cabeca);
    arruma_arquivo(cabeca, arquivo_pronto);
    deleta_lista(cabeca);
    // erro = verifica_nomes(arquivo_pronto);
    // if (erro == -1)
    // {    
    //     printf("Parece que ah um erro em algum nome, de uma olhada no arquivo de erros.\n");
    //     return -1;
    // }
}