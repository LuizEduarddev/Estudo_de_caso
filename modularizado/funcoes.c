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
    int coluna;
    int qntd_parametro;
    char parametro_1[BUFFER];
    char parametro_2[BUFFER];
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

void inserir_parametro(no *aux, char *str, int cont)
{
    if (cont == 1)
        strcpy(aux->parametro_1, str);
    else
        strcpy(aux->parametro_2, str);
    aux->qntd_parametro = cont;        
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

int eh_nulo(FILE *arquivo, FILE *erro_log)
{
    if (arquivo == NULL ||  erro_log == NULL)
    {
        if (arquivo == NULL)
        {
            printf("Houve um erro ao abrir o arquivo de codigos.\n");
        }
        if (erro_log == NULL)
        {
            printf("Houve um erro ao abrir o arquivo de log.\n");
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
        cabeca->primeiro = aux->proximo;
        free(aux);
        return auxiliar;
    }
    else
    {
        auxiliar = aux->anterior;
        aux->anterior->proximo = aux->proximo;
        aux->proximo->anterior = aux->anterior ;
        free(aux);
        aux = auxiliar;
        return aux;
    }
}

void arruma_arquivo(gancho *cabeca, FILE *erro_log)
{
    no *aux = final(cabeca);
    no *auxiliar = NULL;
    while (aux != NULL)
    {
        aux = so_espaco(aux, cabeca); 
        if (aux->linha[0] == ' ')
        {
            remove_espaco(aux);
        }
        if (aux->linha[0] == '\n' || aux->linha[0] == '#')
        {
            aux = remove_item(aux, cabeca);
        }
        else
        {
            tem_hashtag(aux);
            pega_parametro(aux, erro_log);
            aux = aux->anterior;
        }
    }

}

int tira_nome(no *aux)
{
    int i = 0, inicio = 0, fim = 0;
    char nome[BUFFER];
    while (aux->linha[i] != ' ')
        i++;

    return i;    
}

int pega_parametro(no *aux, FILE *erro_log)
{   
    char parametro[BUFFER];
    int i = 0, fim = 0, inicio = 0, tamanho = 0, cont = 1;
    i = tira_nome(aux);
    while (aux->linha[i] != '\0')
    {
        while(aux->linha[i] == ' ')
            i++;  
        
        inicio = i;

        while(aux->linha[i] != ' ' && aux->linha[i] != '\0' && aux->linha[i] != '\n')
            i++;
  
        fim = i;

        strncpy(parametro, aux->linha + inicio, fim - inicio);
        parametro[fim - inicio] = '\0';
        inserir_parametro(aux, parametro, cont);

        cont++;
    }
}


int verifica_args(gancho *cabeca, FILE *arquivo, FILE *erro_log)
{
    no *aux = final(cabeca);
    int numero_func;
    int erro;
    while (aux != NULL)
    {
        numero_func = retorna_nome(aux);
        if (numero_func == 1)
        {
            erro = um_paramentro(aux, arquivo, erro_log);
            if (erro == -1)
                return -1;
        }

        if (numero_func == 2)
        {
            erro = dois_paramentros(aux, arquivo, erro_log);
            if (erro == -1)
                return -1;
        }

        if (numero_func == 3)  
        {
            erro = storeconst(aux, arquivo, erro_log);
            if (erro == -1)
                return -1;
        }

        if (numero_func == 4)
        {
            erro = jump(aux, arquivo, erro_log, cabeca);
            if (erro == -1)
                return -1;
        }
        aux = aux->anterior;
    }
    fprintf(erro_log, "Nenhum erro para ser mostrado no momento.\n");
}

int um_paramentro(no *aux, FILE *arquivo, FILE *erro_log)
{
    int i=0, j=0, cont_parametro=0, espaco=0;
    char *funcao = nome_func(aux);
    for (i=0; aux->linha[i] != '\0'; i++)
    {
        if (aux->linha[i] == ' ')
        {
            if ((espaco = args_espaco(aux, i)) == 0)
            {
                for (j = i+1; aux->linha[j] != ' '; j++)
                {
                    if (!isdigit(aux->linha[j]))
                    {
                        if(aux->linha[j] == '\n')
                        {
                            break;
                        }
                        else
                        {
                            printf("Um erro ocorreu, olhe o painel de log para mais informacoes.\n");
                            fprintf(erro_log, "Erro coluna %d: A funcao '%s' so aceita numeros inteiros em seus parametros.", aux->coluna, funcao);
                            return -1;
                        }
                    }
                }
                cont_parametro++;
                i = j;
            }
        }
    } 
    if (cont_parametro < 1)
    {
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        fprintf(erro_log, "Erro coluna %d: A funcao '%s' precisa de pelo menos um parametro, mas foram inseridos '%d'.", aux->coluna, funcao, cont_parametro);
        return -1;
    }
    if (cont_parametro > 1)
    {
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        fprintf(erro_log, "Erro coluna %d: A funcao '%s' precisa de apenas um parametro, mas foram inseridos '%d'.", aux->coluna, funcao, cont_parametro);
        return -1;
    }
}

int dois_paramentros(no *aux, FILE *arquivo, FILE *erro_log)
{
    int i=0, j=0, cont_parametro=0, espaco=0;
    char *funcao = nome_func(aux);
    for (i=0; aux->linha[i] != '\0'; i++)
    {
        if (aux->linha[i] == ' ')
        {
            if ((espaco = args_espaco(aux, i)) == 0)
            {
                for (j = i+1; aux->linha[j] != '\0'; j++)
                {
                    if (aux->linha[j] != ' ')
                    {
                        if (isdigit(aux->linha[j+1]))
                        {
                            continue;
                        }
                        else
                        {
                            if (!isdigit(aux->linha[j]))
                            {
                                if(aux->linha[j] == '\n')
                                {
                                    i = j;
                                    break;
                                }
                                else
                                {
                                    printf("Um erro ocorreu, olhe o painel de log para mais informacoes.\n");
                                    fprintf(erro_log, "Erro coluna %d: A funcao '%s' so aceita numeros inteiros em seus parametros.", aux->coluna, funcao);
                                    return -1;
                                }
                            }
                            cont_parametro++;
                        }
                    }
                }
            }
        }
    } 
    if (cont_parametro < 2)
    {
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        fprintf(erro_log, "Erro coluna %d: A funcao '%s' precisa de pelo menos dois parametros, mas foram inseridos '%d'.", aux->coluna, funcao, cont_parametro);
        return -1;
    }
    if (cont_parametro > 2)
    {
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        fprintf(erro_log, "Erro coluna %d: A funcao '%s' precisa de apenas dois parametros, mas foram inseridos '%d'.", aux->coluna, funcao, cont_parametro);
        return -1;
    }
}

int storeconst(no *aux, FILE *arquivo, FILE *erro_log)
{
    int i=0, j=0, cont_parametro=0, espaco=0, cont_arg_float = 0;
    char *funcao = nome_func(aux);
    for (i=0; aux->linha[i] != '\0'; i++)
    {
        if (aux->linha[i] == ' ')
        {
            if (cont_arg_float == 0)
            {
                if ((espaco = args_espaco(aux, i)) == 0)
                {
                    for (j = i+1; aux->linha[j] != '\0'; j++)
                    {
                        if (aux->linha[j] != ' ')
                        {
                            if (isdigit(aux->linha[j+2]))
                            {
                                continue;
                            }
                            else
                            {
                                if (!isdigit(aux->linha[j]))
                                {
                                    if(aux->linha[j] == '\n')
                                    {
                                        i = j;
                                        break;
                                    }
                                    if (aux->linha[j] == '-')
                                    {
                                        continue;
                                    }
                                    if (aux->linha[j] == '.')
                                    {
                                        continue;
                                    }
                                    else
                                    {
                                        printf("Um erro ocorreu, olhe o painel de log para mais informacoes.\n");
                                        fprintf(erro_log, "Erro coluna %d: A funcao '%s' so aceita numeros inteiros em seus parametros.", aux->coluna, funcao);
                                        return -1;
                                    }
                                }
                                cont_parametro++;
                            }
                        }
                    }
                }
            }
            if (cont_arg_float > 0)
            {
                if ((espaco = args_espaco(aux, i)) == 0)
                {
                    for (j = i+1; aux->linha[j] != '\0'; j++)
                    {
                        if (aux->linha[j] != ' ')
                        {
                            if (isdigit(aux->linha[j+1]))
                            {
                                continue;
                            }
                            else
                            {
                                if (!isdigit(aux->linha[j]))
                                {
                                    if(aux->linha[j] == '\n')
                                    {
                                        i = j;
                                        break;
                                    }
                                    else
                                    {
                                        printf("Um erro ocorreu, olhe o painel de log para mais informacoes.\n");
                                        fprintf(erro_log, "Erro coluna %d: A funcao '%s' so aceita numeros inteiros em seus parametros.", aux->coluna, funcao);
                                        return -1;
                                    }
                                }
                                cont_parametro++;
                            }
                        }
                    }
                }
            }
            cont_arg_float++;
        }
    } 
    if (cont_parametro < 2)
    {
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        fprintf(erro_log, "Erro coluna %d: A funcao '%s' precisa de pelo menos dois parametros, mas foram inseridos '%d'.", aux->coluna, funcao, cont_parametro);
        return -1;
    }
    if (cont_parametro > 2)
    {
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        fprintf(erro_log, "Erro coluna %d: A funcao '%s' precisa de apenas dois parametros, mas foram inseridos '%d'.", aux->coluna, funcao, cont_parametro);
        return -1;
    }
}

int args_espaco(no *aux, int n)
{
    for(int i=n; aux->linha[i] != '\0' ; i++)
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

char *nome_func(no *aux)
{
    int i=0;
    char *funcao;
    for (i=0; aux->linha[i] != ' '; i++);
    strncpy(funcao, aux->linha, i);
    funcao[i] = '\0';
    
    if (strcmp(funcao, "read")==0)
    { 
        return funcao; 
    }
    if (strcmp(funcao, "storeconst")==0)
    {
        return funcao;
    }
    if (strcmp(funcao, "add")==0)
    {
        return funcao;
    }
    if (strcmp(funcao, "store")==0)
    {
        return funcao;
    }
    if(strcmp(funcao, "sub")==0)
    {
        return funcao;
    }
    if(strcmp(funcao, "write")==0)
    {
        return funcao;
    }
    if(strcmp(funcao, "jump")==0)
    {
        return funcao;   
    }
    if(strcmp(funcao, "div")==0)
    {
        return funcao;   
    }
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

int retorna_nome(no *aux)
{
    int i=0;
    char funcao[20];
    for (i=0; aux->linha[i] != ' '; i++);
    strncpy(funcao, aux->linha, i);
    funcao[i] = '\0';
    
    if (strcmp(funcao, "read")==0)
    { 
        return 1; 
    }
    if (strcmp(funcao, "storeconst")==0)
    {
        return 3;
    }
    if (strcmp(funcao, "add")==0)
    {
        return 2;
    }
    if (strcmp(funcao, "store")==0)
    {
        return 1;
    }
    if(strcmp(funcao, "sub")==0)
    {
        return 2;
    }
    if(strcmp(funcao, "write")==0)
    {
        return 1;
    }
    if(strcmp(funcao, "jump")==0)
    {
        return 4;   
    }
    if(strcmp(funcao, "div")==0)
    {
        return 2;   
    }
    return 0;
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
        if ((erro = retorna_nome(aux)) == 0)
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

int jump(no *aux, FILE *arquivo, FILE *erro_log, gancho *cabeca)
{
    int i=0, j=0, cont_parametro=0, espaco=0, jump_pula = 0, cont_arg_float = 0, z = 0, erro = 0;
    for (i=0; aux->linha[i] != '\0'; i++)
    {
        if (aux->linha[i] == ' ')
        {
           if (cont_arg_float == 0)
            {
                if ((espaco = args_espaco(aux, i)) == 0)
                {
                    for (j = i+1; aux->linha[j] != '\0'; j++)
                    {
                        if (aux->linha[j] != ' ')
                        {
                            if (cont_arg_float == 0)
                            {
                                if (isdigit(aux->linha[j+1]))
                                {
                                    continue;
                                }
                                else
                                {
                                    if (!isdigit(aux->linha[j]))
                                    {
                                        if(aux->linha[j] == '\n')
                                        {
                                            i = j;
                                            break;
                                        }
                                        else
                                        {
                                            printf("Um erro ocorreu, olhe o painel de log para mais informacoes.\n");
                                            fprintf(erro_log, "Erro coluna %d: A funcao 'jump' so aceita numeros inteiros em seus parametros.", aux->coluna);
                                            return -1;
                                        }
                                    }
                                    cont_parametro++;
                                    cont_arg_float++;
                                }
                            }
                            else
                            {
                                if (cont_arg_float == 1)
                                {
                                    if ((espaco = args_espaco(aux, i)) == 0)
                                    {
                                        z = j;
                                        for (j = z; aux->linha[j] != '\0'; j++)
                                        {
                                            if (aux->linha[j] != ' ')
                                            {
                                                if (isdigit(aux->linha[j+1]))
                                                {
                                                    jump_pula = pega_int(aux, j);
                                                    cont_arg_float++;
                                                    cont_parametro++;
                                                    break;
                                                }
                                                else
                                                {
                                                    if (!isdigit(aux->linha[j]))
                                                    {
                                                        if(aux->linha[j] == '\n')
                                                        {
                                                            i = j;
                                                            break;
                                                        }
                                                        else
                                                        {
                                                            printf("Um erro ocorreu, olhe o painel de log para mais informacoes.\n");
                                                            fprintf(erro_log, "Erro coluna %d: A funcao 'jump' so aceita numeros inteiros em seus parametros.", aux->coluna);
                                                            return -1;
                                                        }
                                                    }
                                                    jump_pula = aux->linha[j] - '0';
                                                    cont_parametro++;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } 
    if (cont_parametro < 2)
    {
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        fprintf(erro_log, "Erro coluna %d: A funcao 'jump' precisa de pelo menos dois parametros, mas foram inseridos '%d'.", aux->coluna, cont_parametro);
        return -1;
    }
    if (cont_parametro > 2)
    {
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        fprintf(erro_log, "Erro coluna %d: A funcao 'jump' precisa de apenas dois parametros, mas foram inseridos '%d'.", aux->coluna, cont_parametro);
        return -1;
    }
    erro = confere_jump(cabeca, jump_pula, erro_log, aux->coluna);
    if (erro == -1)
        return -1;
}

int pega_int(no *aux, int j)
{
    char str_to_n[BUFFER];
    int z = 0;
    int numero;
    for (int i = j; aux->linha[i] != ' '; i++)
    {
        str_to_n[z] = aux->linha[i];
        z++;
    }
    numero = strtol(str_to_n, NULL, 10);
    return numero;

}

int confere_jump(gancho *cabeca, int jump_pula, FILE *erro_log, int col)
{
    no *atual = final(cabeca);
    int maior = -999;
    int resultado = 0;
    while (atual !=  NULL)
    {
        if (atual->coluna > maior)
        {
            maior = atual->coluna;
        }
        atual = atual->anterior;
    }
    resultado = col + jump_pula;
    if (resultado < 0)
    {
        printf("Ocorreu um erro, abra o arquivo de log para mais informacoes.\n");
        fprintf(erro_log, "Erro coluna %d: a funcao 'jump' nao pode voltar uma quantidade de colunas menor do que suas colunas.", col);
        return -1;
    }
    if (resultado > maior)
        {
            printf("Ocorreu um erro, abra o arquivo de log para mais informacoes.\n");
            fprintf(erro_log, "Erro coluna %d: a funcao 'jump' nao pode avancar uma quantidade de colunas maior do que suas colunas.", col);
            return -1;
        } 
    else
    {
        return 0;
    }

}