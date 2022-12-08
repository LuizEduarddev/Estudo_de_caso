#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include<conio.h>
#include<ctype.h>
#include "funcoes.h"
#define BUFFER 100
#define NEGATIVO -1
#define POSITIVO 1
#define N_FLOAT 2
#define DIFERENTE_NUMERO 0
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
    int qntd_colunas;
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
    cabeca->qntd_colunas = col;
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
        
        if (aux->linha[i] == '\n' || aux->linha[i] == '\0')
            break;
        
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
    int *vetor = (int *)(malloc(BUFFER * sizeof(int)));
    int *store = (int *)(malloc(sizeof(int)));
    int numero_func;
    int erro;
    char *funcao;
    while (aux != NULL)
    {
        funcao = nome_func(aux);
        numero_func = retorna_nome(funcao);
        if (numero_func == 1)
        {
            erro = um_paramentro(aux, arquivo, erro_log, vetor, store);
            if (erro == -1)
                return -1;
        }

        if (numero_func == 2)
        {
            erro = dois_parametros(aux, arquivo, erro_log, vetor, store);
            if (erro == -1)
                return -1;
        }

        if (numero_func == 3)  
        {
            erro = storeconst(aux, arquivo, erro_log, vetor, store);
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

int um_paramentro(no *aux, FILE *arquivo, FILE *erro_log, int *vetor, int *store)
{
    int tipo = 0;
    char *funcao = nome_func(aux);
    char nome[BUFFER];
    strcpy(nome, funcao);
    if (aux->qntd_parametro != 1)
    {
        printf("Um erro ocorreu, olhe o painel de log para mais informacoes.\n");
        fprintf(erro_log, "ERRO linha %d: A funcao '%s' precisa de apenas 1 parametro mas recebeu %d.", aux->coluna, nome, aux->qntd_parametro);
        return -1;
    }
    if (tipo = n_tipo(aux->parametro_1) != POSITIVO)
    {
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        fprintf(erro_log, "ERRO linha %d: a funcao '%s' aceita apenas numeros naturais positivos. Recebido %s", aux->coluna, nome, aux->parametro_1);
        return -1;
    }
    realiza_func(aux, nome, vetor, store);
    return 0;
}

int dois_parametros(no *aux, FILE *arquivo, FILE *erro_log, int *vetor, int *store)
{
    int tipo = 0;
    char *funcao = nome_func(aux);
    char nome[BUFFER];
    strcpy(nome, funcao);
    if (aux->qntd_parametro != 2)
    {
        printf("Um erro ocorreu, olhe o painel de log para mais informacoes.\n");
        fprintf(erro_log, "ERRO linha %d: A funcao '%s' precisa de apenas 2 parametros mas recebeu %d.", aux->coluna, nome, aux->qntd_parametro);
        return -1;
    }

    if ((tipo = n_tipo(aux->parametro_1)) != POSITIVO || (tipo = n_tipo(aux->parametro_2)) !=  POSITIVO)
    {
        fprintf(erro_log, "Linha %d: a funcao '%s' nao aceita valores diferentes de numeros naturais positivos. Recebido %s.", aux->coluna, nome, aux->parametro_1);
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        return -1;
    }
    realiza_func(aux, nome, vetor, store);
    return 0;
}

int storeconst(no *aux, FILE *arquivo, FILE *erro_log, int *vetor, int *store)
{
    int tipo = 0;
    char *funcao = nome_func(aux);
    char nome[BUFFER];
    strcpy(nome, funcao);
    if (aux->qntd_parametro != 2)
    {
        printf("Um erro ocorreu, olhe o painel de log para mais informacoes.\n");
        fprintf(erro_log, "ERRO linha %d: A funcao '%s' precisa de apenas 2 parametros mas recebeu %d.", aux->coluna, nome, aux->qntd_parametro);
        return -1;
    }

    if (tipo = n_tipo(aux->parametro_1) == DIFERENTE_NUMERO)
    {
        fprintf(erro_log, "Linha %d: a funcao '%s' nao aceita valores diferentes de numeros. Recebeu %s.", aux->coluna, nome, aux->parametro_1);
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        return -1;
    }
    if (tipo = n_tipo(aux->parametro_2) == DIFERENTE_NUMERO)
    {
        fprintf(erro_log, "Linha %d: a funcao '%s' nao aceita valores diferentes de numeros. Recebeu %s.", aux->coluna, nome, aux->parametro_2);
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        return -1;
    }

    if ((tipo = n_tipo(aux->parametro_2)) == NEGATIVO)
    {
        fprintf(erro_log, "Linha %d: a funcao '%s' nao aceita valores negativos como segundo argumento. Recebeu %s.", aux->coluna, nome, aux->parametro_2);
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        return -1;
    }

    if ((tipo = n_tipo(aux->parametro_2)) == N_FLOAT)
    {
        fprintf(erro_log, "Linha %d: a funcao '%s' nao aceita valores do tipo real como segundo argumento. Recebeu %d.", aux->coluna, nome, aux->parametro_2);
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        return -1;
    }
    realiza_func(aux, nome, vetor, store);
    return 0;
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
    char funcao_nome[BUFFER];
    char *funcao;
    for (i=0; aux->linha[i] != ' '; i++);
        strncpy(funcao_nome, aux->linha, i);
    
    funcao_nome[i] = '\0';
    funcao = funcao_nome;
    
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
    if(strcmp(funcao, "mult")==0)
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

int retorna_nome(char *funcao)
{
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
    if(strcmp(funcao, "mult")==0)
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
    char* funcao;
    while(aux != NULL)
    {
        funcao = nome_func(aux);

        if ((erro = retorna_nome(funcao)) == 0)
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
    int tipo = 0;
    int erro = 0;
    char *funcao = nome_func(aux);
    int n;
    if (aux->qntd_parametro < 2)
    {
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        fprintf(erro_log, "Erro linha %d: A funcao '%s' precisa de pelo menos dois parametros.", aux->coluna, funcao);
        return -1;
    }
    if (aux->qntd_parametro > 2)
    {
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        fprintf(erro_log, "Erro linha %d: A funcao '%s' precisa de apenas dois parametro", aux->coluna, funcao);
        return -1;
    }

    if ((tipo = n_tipo(aux->parametro_1)) == DIFERENTE_NUMERO || (tipo = n_tipo(aux->parametro_2)) == DIFERENTE_NUMERO)
    {
        fprintf(erro_log, "Linha %d: a funcao '%s' nao aceita valores diferentes de numeros.", aux->coluna, funcao);
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        return -1;
    }

    if ((tipo = n_tipo(aux->parametro_1)) == NEGATIVO)
    {
        fprintf(erro_log, "Linha %d: a funcao '%s' nao aceita valores negativos.", aux->coluna, funcao);
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        return -1;
    }

    if ((tipo = n_tipo(aux->parametro_1)) == N_FLOAT || (tipo = n_tipo(aux->parametro_1)) == N_FLOAT)
    {
        fprintf(erro_log, "Linha %d: a funcao '%s' nao aceita valores do tipo real.", aux->coluna, funcao);
        printf("Um erro ocorreu, olhe o painel de logs para mais informacoes.\n");
        return -1;
    }

    erro = confere_jump(aux->coluna, arquivo, erro_log, n = para_jump(aux->parametro_2), cabeca->qntd_colunas);
    if (erro == -1) 
        return -1;
}

int confere_jump(int col, FILE *arquivo, FILE *erro_log, int jump_pula, int maior)
{
    int resultado = col + jump_pula;
    if (resultado < 0)
    {
        printf("Ocorreu um erro, abra o arquivo de log para mais informacoes.\n");
        fprintf(erro_log, "Erro linha %d: a funcao 'jump' nao pode voltar uma quantidade de colunas menor do que suas colunas.", col);
        return -1;
    }
    if (resultado > maior)
    {
        printf("Ocorreu um erro, abra o arquivo de log para mais informacoes.\n");
        fprintf(erro_log, "Erro linha %d: a funcao 'jump' nao pode avancar uma quantidade de colunas maior do que suas colunas.", col);
        return -1;
    } 
    else
    {
        return 0;
    }
}

int n_tipo(char *str)
{
    char *final;
    int numero_int = strtol(str, &final, 10);
    if (*final == '\0')
    {
        if (numero_int >= 0)
            return POSITIVO;

        return NEGATIVO;    
    }
    float numero_float = strtod(str, &final);
    if (*final == '\0')
        return N_FLOAT;

    return DIFERENTE_NUMERO;    
}

int para_jump(char *str)
{
    char *final;
    int numero_int = strtol(str, &final, 10);
    return numero_int;
}

void realiza_func(no *aux, char *nome, int *vetor, int *store)
{
    if (strcmp(nome, "read") == 0)
        funcao_read(aux, vetor);
    if (strcmp(nome, "storeconst") == 0)
        funcao_storeconst(aux, vetor);
    if (strcmp(nome, "add") == 0)
        *(store) = funcao_add(aux, vetor);
    if (strcmp(nome, "sub") == 0)
        *(store) = funcao_sub(aux, vetor);
    if (strcmp(nome, "mult") == 0)
        *(store) = funcao_mult(aux, vetor);    
    if (strcmp(nome, "store") == 0)
        funcao_store(aux, vetor, store);
    if (strcmp(nome, "div") == 0)
        *(store) = funcao_div(aux, vetor);
    if (strcmp(nome, "write") == 0)
        funcao_write(aux, vetor);
}

void funcao_read(no *aux, int *vetor)
{
    int numero = para_jump(aux->parametro_1);
    if (numero > BUFFER)
        printf("WARNING! O parametro e maior do que a quantidade alocada para o vetor, esta acao resultara em um comportamento indeterminado do sistema.\n");

    printf("Funcao READ inicializada.\n");
    printf("Digite o numero que sera colocado na posicao %d da memoria: ", numero);
    scanf("%d", vetor + numero);    

    printf("Numero '%d' alocado no vetor.\n", *(vetor + numero));
}

void funcao_storeconst(no *aux, int *vetor)
{
    int numero = para_jump(aux->parametro_1);
    int pos = para_jump(aux->parametro_2);

    if (pos > BUFFER)
        printf("WARNING! O parametro e maior do que a quantidade alocada para o vetor, esta acao resultara em um comportamento indeterminado do sistema.\n");

    printf("funcao STORECONST inicializada.\n");
    *(vetor + pos) = numero;
    printf("O numero '%d' foi alocado na pos '%d'.\n", *(vetor + pos), pos);
}

int funcao_add(no *aux, int *vetor)
{
    int pos1 = para_jump(aux->parametro_1);
    int pos2 = para_jump(aux->parametro_2);

    if (pos1 > BUFFER || pos2 > BUFFER)
        printf("WARNING! O parametro e maior do que a quantidade alocada para o vetor, esta acao resultara em um comportamento indeterminado do sistema.\n");

    printf("funcao ADD inicializada.\n");
    printf("Realizando a soma de %d com %d.\n", *(vetor + pos1), *(vetor + pos2));
    printf("O resultado eh %d, ele foi pre-alocado, caso deseje adiciona-lo ao vetor, coloque o comando 'store'.\n", *(vetor + pos1) + *(vetor + pos2));
    return *(vetor + pos1) + *(vetor + pos2);
}

int funcao_sub(no *aux, int *vetor)
{
    int pos1 = para_jump(aux->parametro_1);
    int pos2 = para_jump(aux->parametro_2);

    if (pos1 > BUFFER || pos2 > BUFFER)
        printf("WARNING! O parametro e maior do que a quantidade alocada para o vetor, esta acao resultara em um comportamento indeterminado do sistema.\n");

    printf("funcao SUB inicializada.\n");
    printf("Realizando a subtracao de %d com %d.\n", *(vetor + pos1), *(vetor + pos2));
    printf("O resultado eh %d, ele foi pre-alocado, caso deseje adiciona-lo ao vetor, coloque o comando 'store'.\n", *(vetor + pos1) - *(vetor + pos2));
    return *(vetor + pos1) - *(vetor + pos2);
}

int funcao_div(no *aux, int *vetor)
{
    int pos1 = para_jump(aux->parametro_1);
    int pos2 = para_jump(aux->parametro_2);

    if (pos1 > BUFFER || pos2 > BUFFER)
        printf("WARNING! O parametro e maior do que a quantidade alocada para o vetor, esta acao resultara em um comportamento indeterminado do sistema.\n");

    printf("funcao DIV inicializada.\n");
    printf("Realizando a divisao de %d com %d.\n", *(vetor + pos1), *(vetor + pos2));
    printf("O resultado eh %d, ele foi pre-alocado, caso deseje adiciona-lo ao vetor, coloque o comando 'store'.\n", *(vetor + pos1) / *(vetor + pos2));
    return *(vetor + pos1) / *(vetor + pos2);
}

int funcao_mult(no *aux, int *vetor)
{
    int pos1 = para_jump(aux->parametro_1);
    int pos2 = para_jump(aux->parametro_2);

    if (pos1 > BUFFER || pos2 > BUFFER)
        printf("WARNING! O parametro e maior do que a quantidade alocada para o vetor, esta acao resultara em um comportamento indeterminado do sistema.\n");

    printf("funcao MULT inicializada.\n");
    printf("Realizando a multiplicacao de %d com %d.\n", *(vetor + pos1), *(vetor + pos2));
    printf("O resultado eh %d, ele foi pre-alocado, caso deseje adiciona-lo ao vetor, coloque o comando 'store'.\n", *(vetor + pos1) * *(vetor + pos2));
    return *(vetor + pos1) * *(vetor + pos2);
}

void funcao_store(no *aux, int *vetor, int *store)
{
    int pos = para_jump(aux->parametro_1);
    if (pos > BUFFER)
        printf("WARNING! O parametro e maior do que a quantidade alocada para o vetor, esta acao resultara em um comportamento indeterminado do sistema.\n");

    printf("Funcao STORE inicializada.\n");    
    *(vetor + pos) = *(store);
    printf("O resultado %d deixou de ser pre alocado e foi alocado no vetor, na pos %d", *(vetor + pos), pos);
}

void funcao_write(no *aux, int *vetor)
{
    int pos = para_jump(aux->parametro_1);

    printf("Funcao WRITE inicializada.\n");    
    printf("O que esta na posicao %d eh '%d'.\n", pos, *(vetor + pos));
}