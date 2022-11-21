#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include<conio.h>
#include<ctype.h>
#include "funcoes.h"
#define BUFFER 100

struct no
{
    char linha[BUFFER];   
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

void inserir_lista(gancho *cabeca, char *str)
{
    no* novo = (no *)(malloc(sizeof(no)));
    strcpy(novo->linha, str);
    novo->proximo = cabeca->primeiro;
    novo->anterior = NULL;
    if (cabeca->primeiro != NULL)
    {
        cabeca->primeiro->anterior = novo;
    }
    cabeca->primeiro = novo;
}

int eh_nulo(FILE *arquivo, FILE *arquivo_pronto, FILE *erro_log)
{
    if (arquivo == NULL || arquivo_pronto == NULL || erro_log == NULL)
    {
        if (arquivo == NULL)
        {
            printf("Houve um erro ao abrir o arquivo principal.\n");
        }
        if (arquivo_pronto == NULL)
        {
            printf("Houve um erro ao abrir o arquivo organizado.\n");
        }
        if (erro_log == NULL)
        {
            printf("Houve um erro ao abrir o arquivo de erro.\n");
        }
    }
    else
    {
        return 0;
    }
}

void inserir_arquivo(FILE *arquivo, gancho *cabeca)
{
    char str[BUFFER];
    while (!feof(arquivo))
    {
        fgets(str, BUFFER, arquivo);
        inserir_lista(cabeca, str);
    }
}

void arruma_arquivo(gancho *cabeca, FILE *arquivo_pronto)
{
    no *atual = cabeca->primeiro;
    no *aux = NULL;
    int espaco = 0;
    while (atual != NULL)
    {
        aux = atual;
        atual = atual->proximo;
    }
    while (aux != NULL)
    {
        for (int i=0; aux->linha[i] != '\0'; i++)
        {
            if (aux->linha[0] == ' ')
            {
                espaco = so_espaco(aux);
                if (espaco == 0)
                {
                    remove_espaco(aux);
                    fprintf(arquivo_pronto, aux->linha);
                }
                else
                {
                    aux = aux->anterior;
                }
            }
            else if (aux->linha[0] == '\n')
                aux = aux->anterior;
            else if (aux->linha[i] == '#')
            {
                aux->linha[i] = '\0';
                if (aux->linha[i] == '\0')
                {    
                    if (aux->linha[i - 1] == ' ')
                        aux->linha[i - 1] = '\0'; 
                }   
                i -= 1;
            }
        }
        fprintf(arquivo_pronto, aux->linha);
        aux = aux->anterior;
    }
}

int so_espaco(no *aux)
{
    for(int i=0; aux->linha[i] != '\0' ; i++)
    {
        if (!isdigit(aux->linha[i]))
        {
            if (aux->linha[i] == ' ')
            {
                continue;
            }
            else if (aux->linha[i] != '\n')
            {
                return 0;
            }
        }
        if (isdigit(aux->linha[i]))
        {
            return 0;
        }
    }
    return -1;
}

void remove_espaco(no *aux)
{
    char str[50];
    int inicio = 0;
    int j=0;
    int tamanho;
    for (int i=0; aux->linha[i] != '\0'; i++)
    {
        if (aux->linha[i] != ' ')
        {
            inicio = i;
            break;
        }
    }
    for (int i = inicio; aux->linha[i] != '\0'; i++)
    {
        str[j] = aux->linha[i];
        j++;   
    }
    str[j] = '\0';
    strcpy(aux->linha, str);
    return;
}

void deleta_lista(gancho *cabeca)
{
    no *aux = NULL;
    no *atual = cabeca->primeiro;

    while (atual != NULL)
    {
        aux = atual->proximo;
        free(atual);
        atual = aux;
    }
}

void inserir_arquivo_pronto(gancho *cabeca, FILE *arquivo_pronto)
{

}

// int verifica_nomes(FILE *arquivo_pronto)
// {
//     int i = 0;
//     char funcao[20];
//     for (i=0; [i] != ' '; i++);
//         strncpy(funcao, nome_func, i);
//     funcao[i] = '\0';
    
//     while(1)
//     {
//         if (strcmp(funcao, "read")==0)
//         { 
//             return 0; 
//         }
//         if (strcmp(funcao, "storeconst")==0)
//         {
//             return 0;
//         }
//         if (strcmp(funcao, "add")==0)
//         {
//             return 0;
//         }
//         if (strcmp(funcao, "store")==0)
//         {
//             return 0;
//         }
//         if(strcmp(funcao, "sub")==0)
//         {
//             return 0;
//         }
//         if(strcmp(funcao, "write")==0)
//         {
//             return 0;
//         }
//         if(strcmp(funcao, "jump")==0)
//         {
//             return 0;   
//         }
//         for (i=0; nome_func[i] != '\0'; i++)
//         {
//             if (isdigit(nome_func[i]))
//             {
//                 return -1;
//             }
//         }
//         return -1;
//     }
// }


