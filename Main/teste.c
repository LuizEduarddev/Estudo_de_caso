#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include<conio.h>
#include<ctype.h>
#include "funcoes.h"
#define BUFFER 100

struct no
{
    int linha;
    char funcao[20];   
    no *proximo;
    no *anterior;
};

struct gancho
{
    no *primeiro;
};

gancho* criar_lista(void)
{
    gancho *cabeca = (gancho *)(malloc(sizeof(gancho)));
    cabeca->primeiro = NULL;
    return cabeca;

}
int Inserir_lista(gancho *cabeca, char *vet, int linha)
{
    no* novo = (no *)(malloc(sizeof(no)));
    strcpy(novo->funcao, vet);
    novo->proximo = cabeca->primeiro;
    novo->anterior = NULL;
    if (cabeca->primeiro != NULL)
    {
        cabeca->primeiro->anterior = novo;
    }
    cabeca->primeiro = novo;
    linha++;
    return linha;
}

void arruma_arquivo(FILE *arquivo, FILE *arq_pronto)
{
    char linha[BUFFER];

    while (!feof(arquivo))
    {
        fgets(linha, BUFFER, arquivo);
        for (int i=0; linha[i] != '\0'; i++)
        {
            if (linha[0] == '\n')
            {
                continue;
            }
            if (linha[i] == '#')
            {
                linha[i] == '\0';
            }
        }
        fprintf(arq_pronto, linha);
    }
}