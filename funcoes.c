#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include<conio.h>
#include<ctype.h>
#include "funcoes.h"
#define BUFFER 100

struct no
{
    int coluna;
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

void inserir_lista(gancho *cabeca, char *str, int col)
{
    no* novo = (no *)(malloc(sizeof(no)));
    novo->coluna = col;
    strcpy(novo->linha, str);
    novo->proximo = cabeca->primeiro;
    novo->anterior = NULL;
    if (cabeca->primeiro != NULL)
    {
        cabeca->primeiro->anterior = novo;
    }
    cabeca->primeiro = novo;
}

no *final(gancho *cabeca)
{
    no *atual = cabeca->primeiro;
    no *aux = NULL;
    while (atual != NULL)
    {
        aux = atual;
        atual = atual->proximo;
    }
    return aux;
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
    int coluna = 1;
    while (!feof(arquivo))
    {
        fgets(str, BUFFER, arquivo);
        inserir_lista(cabeca, str, coluna);
        coluna++;
    }
}

no *remove_item(no *aux, gancho *cabeca)
{
    no *auxiliar = NULL;
    if (aux->proximo == NULL)
    {
        auxiliar = aux->anterior;
        aux->anterior->proximo = aux->proximo;
        aux->proximo = NULL;
        free(aux);
        aux = auxiliar;
        return aux;
    }  
    else if (aux->anterior == NULL)
    {
        auxiliar = aux->proximo;
        aux->proximo->anterior = NULL;
        free(aux);
        return auxiliar;
    }
    else
    {
        auxiliar = aux->anterior;
        aux->anterior->proximo = aux->proximo;
        aux->proximo = NULL;
        free(aux);
        aux = auxiliar;
        return aux;
    }
}

void arruma_arquivo(gancho *cabeca)
{
    no *aux = final(cabeca);
    no *auxiliar = NULL;
    while (aux != NULL)
    {
        aux = so_espaco(aux, cabeca); 
        remove_espaco(aux);
        if (aux->linha[0] == '\n' || aux->linha[0] == '#')
        {
            aux = remove_item(aux, cabeca);
        }
        else
        {
            tem_hashtag(aux);
            aux = aux->anterior;
        }
    }
}

void tem_hashtag(no *aux)
{
    for (int i=0; aux->linha[i] != '\0'; i++)
    {
        if (aux->linha[i] == '#')
        {
            if (aux->linha[i-1] == ' ')
            {
                aux->linha[i] = '\0';
                aux->linha[i - 1] = '\n'; 
                i -= 1;
            } 
            else
            {
                aux->linha[i] = '\n';
                aux->linha[i + 1] = '\0';
            }
        } 
    }
}

no *so_espaco(no *atual, gancho *cabeca)
{
    no *aux = NULL;
    for(int i=0; atual->linha[i] != '\0' ; i++)
    {
        if (!isdigit(atual->linha[i]))
        {
            if (atual->linha[i] == ' ')
            {
                continue;
            }
            else if (atual->linha[i] != '\n')
            {
                return atual;
            }
        }
        if (isdigit(atual->linha[i]))
        {
            return atual;
        }
    }
    atual = remove_item(atual, cabeca);
    return atual;
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
    cabeca->primeiro = NULL;
}

int retorna_nome(char *funcao)
{
    if (strcmp(funcao, "read")==0)
    { 
        return 0; 
    }
    if (strcmp(funcao, "storeconst")==0)
    {
        return 0;
    }
    if (strcmp(funcao, "add")==0)
    {
        return 0;
    }
    if (strcmp(funcao, "store")==0)
    {
        return 0;
    }
    if(strcmp(funcao, "sub")==0)
    {
        return 0;
    }
    if(strcmp(funcao, "write")==0)
    {
        return 0;
    }
    if(strcmp(funcao, "jump")==0)
    {
        return 0;   
    }
    if(strcmp(funcao, "div")==0)
    {
        return 0;   
    }
}

void remove_enter(char *str)
{
    for (int i=0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
        }
    }
}

int verifica_nomes(gancho *cabeca, FILE *erro_log)
{
    int i=0, erro = 0, coluna = 1;
    no *aux = final(cabeca);
    char funcao[20];
    while(aux != NULL)
    {
        for (i=0; aux->linha[i] != ' '; i++);
        strncpy(funcao, aux->linha, i);
        funcao[i] = '\0';
        if ((erro = retorna_nome(funcao)) != 0)
        {
            remove_enter(funcao);
            printf("Erro, abra o arquivo log para verificar.\n");
            fprintf(erro_log, "'%s' nao e reconhecido como um comando interno na linha '%d'.\n", funcao, aux->coluna);
            return -1; 
        }
        else
        {
            aux = aux->anterior; 
        }
        coluna++;
    }    
}


