#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include<conio.h>
#include<ctype.h>
#include "funcoes.h"
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
void printar_lista(gancho *cabeca)
{
    no *atual = cabeca->primeiro;
    no *auxiliar = NULL;
    if (atual == NULL)
    {
        printf("Nao a elementos na lista.\n");
        return;
    }
    while (1)
    {
        auxiliar = atual;
        atual = atual->proximo;
        if (atual == NULL)
        {
            while (auxiliar != NULL)
            {
                printf("%s", auxiliar->funcao);
                auxiliar = auxiliar->anterior;
            }
            return;
        }
    }
}

int verifica_func(char *nome_func)
{
    int i = 0;
    char funcao[20];
    if (nome_func[0] == '\n')
    {
        printf("Parece que seu arquivo comecou com uma linha vazia, irei corrigir para voce.\n");
        return 10;
    }
    else
    {
        for (i=0; nome_func[i] != ' '; i++);
        strncpy(funcao, nome_func, i);
        funcao[i] = '\0';
    }
    while(1)
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
        for (i=0; nome_func[i] != '\0'; i++)
        {
            if (isdigit(nome_func[i]))
            {
                return -1;
            }
        }
        return -1;
    }
}

void remove_enter(char *str)
{
    int tamanho = strlen(str);
    for (int i=0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
        }
    }
}

int verifica_args(char *nome_func, int linha)
{
    fflush(stdin);
    remove_enter(nome_func);
    int i;
    int j=0;
    char funcao[20];
    for (i=0; nome_func[i] != ' '; i++);
    strncpy(funcao, nome_func, i);
    funcao[i] = '\0';
    int erro_log = 0;

    if (strcmp(funcao, "read")==0)
    {
        int cont=0;
        char erro;
        int cont_erro = 0;
        int cont_espaco = 0;
        for (i=0; nome_func[i] != '\0'; i++)
        {
            if (nome_func[i] == ' ')
            {
                for (j=i+1; nome_func[j] != ' '; j++)
                {
                    if (nome_func[j] == '\0')
                    {
                        break;
                    }
                    if (!isdigit(nome_func[j]))
                    {
                        erro = nome_func[j];
                        erro_log = -1;
                        arquivo_log(linha, j, funcao, erro, erro_log);
                        return -1;
                    }
                }    
                cont++;            
                cont_espaco++;
            }
        }
        if (cont > 1)
        {
            erro_log = 1;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        else if (cont < 1)
        {
            erro_log = 0;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        else if (cont == 1)
        {
            return 0;
        }
    }
    if (strcmp(funcao, "storeconst")==0)
    {
        int cont=0;
        char erro;
        int cont_erro = 0;
        int pontos=0;
        int negativo = 0;
        int cont_espaco = 0;
        for (i=0; nome_func[i] != '\0'; i++)
        {
            if (cont_espaco > 2)
            {
                break;
            }
            if (nome_func[i] == ' ')
            {
                if (cont_erro <= 0)
                {
                    for (j=i+1; nome_func[j] != ' '; j++)
                    {
                        if (nome_func[j] == '\0')
                        {
                            break;
                        }
                        if (!isdigit(nome_func[j]))
                        {
                            if (nome_func[j] == '-')
                            {
                                if (isdigit(nome_func[j+1]))
                                {
                                    negativo++;
                                    continue;
                                }
                            }
                            if (negativo > 1)
                            {
                                erro_log = 2;
                                arquivo_log(linha, negativo, funcao, erro, erro_log);
                                return -1;
                            }
                            else if (nome_func[j] != '.')
                            {
                                erro = nome_func[j];
                                erro_log = 3;
                                arquivo_log(linha, j, funcao, erro, erro_log);
                                return 2;
                            }
                            pontos++;
                        }
                    }
                    if (pontos > 1)
                    {
                        erro_log = 3;
                        arquivo_log(linha, j, funcao, erro, erro_log);
                        return 2;
                    }
                    cont++;               
                    cont_espaco++;
                }
                if (cont_erro >= 1)
                {
                    for (j=i+1; nome_func[j] != ' '; j++)
                    {
                        if (nome_func[j] == '\0')
                        {
                            break;
                        }
                        if (!isdigit(nome_func[j]))
                        {
                            erro = nome_func[j];
                            erro_log = 5;
                            arquivo_log(linha, j, funcao, erro, erro_log);
                            return -1;
                        }
                    } 
                    cont++;   
                }
                cont_erro++;
            }   
        }
        if (cont > 2)
        {
            erro_log = 6;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        if (cont < 2)
        {
            erro_log = 7;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        else if (cont == 2)
        {
            return 0;
        }
       
    }
    if (strcmp(funcao, "jump")==0)
    {
        int cont=0;
        char erro;
        int cont_erro = 0;
        int cont_espaco = 0;
        for (i=0; nome_func[i] != '\0'; i++)
        {
            
            if (cont_espaco > 2)
            {
                break;
            }
            if (nome_func[i] == ' ')
            {
                for (j=i+1; nome_func[j] != ' '; j++)
                {
                    if (nome_func[j] == '\0')
                    {
                        break;
                    }
                    if (!isdigit(nome_func[j]))
                    {
                        erro = nome_func[j];
                        erro_log = -1;
                        arquivo_log(linha, j, funcao, erro, erro_log);
                        return -1;
                    }
                }    
                cont++;      
                cont_espaco++;      
            }
        }
        if (cont > 2)
        {
            erro_log = 8;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        if (cont < 2)
        {
            erro_log = 8;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }    
        else if (cont == 2)
        {
            return 0;
        }
    }
    if (strcmp(funcao, "add")==0)
    {
        int cont=0;
        char erro;
        int cont_erro = 0;
        int cont_espaco = 0;
        for (i=0; nome_func[i] != '\0'; i++)
        {
            if (cont_espaco > 2)
            {
                break;
            }
            if (nome_func[i] == ' ')
            {
                for (j=i+1; nome_func[j] != ' '; j++)
                {
                    if (nome_func[j] == '\0')
                    {
                        break;
                    }
                    if (!isdigit(nome_func[j]))
                    {
                        erro = nome_func[j];
                        erro_log = -1;
                        arquivo_log(linha, j, funcao, erro, erro_log);
                        return -1;
                    }
                }    
                cont++;   
                cont_espaco++;         
            }
        }
        if (cont > 2)
        {
            erro_log = 8;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        if (cont < 2)
        {
            erro_log = 8;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        else if (cont == 2)
        {
            return 0;
        }    
    }
    if (strcmp(funcao, "sub")==0)
    {
        int cont=0;
        char erro;
        int cont_erro = 0;
        int cont_espaco = 0;
        for (i=0; nome_func[i] != '\0'; i++)
        {
            if (cont_espaco > 2)
            {
                break;
            }
            if (nome_func[i] == ' ')
            {
                for (j=i+1; nome_func[j] != ' '; j++)
                {
                    if (nome_func[j] == '\0')
                    {
                        break;
                    }
                    if (!isdigit(nome_func[j]))
                    {
                        erro = nome_func[j];
                        erro_log = -1;
                        arquivo_log(linha, j, funcao, erro, erro_log);
                        return -1;
                    }
                }    
                cont++;    
                cont_espaco++;       
            }
        }
        if (cont > 2)
        {
            erro_log = 8;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        if (cont < 2)
        {
            erro_log = 8;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }    
        else if (cont == 2)
        {
            return 0;
        }
    }
    if (strcmp(funcao, "store")==0)
    {
        int cont=0;
        char erro;
        int cont_erro = 0;
        int cont_espaco = 0;
        for (i=0; nome_func[i] != '\0'; i++)
        {
            if (cont_espaco > 1)
            {
                break;
            }
            if (nome_func[i] == ' ')
            {
                for (j=i+1; nome_func[j] != ' '; j++)
                {
                    if (nome_func[j] == '\0')
                    {
                        break;
                    }
                    if (!isdigit(nome_func[j]))
                    {
                        erro = nome_func[j];
                        erro_log = -1;
                        arquivo_log(linha, j, funcao, erro, erro_log);
                        return -1;
                    }
                }    
                cont++;   
                cont_espaco++;         
            }
        }
        if (cont > 1)
        {
            erro_log = 1;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        if (cont < 1)
        {
            erro_log = 1;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        else if (cont == 1)
        {
            return 0;
        }
    }
    if (strcmp(funcao, "write")==0)
    {
        int cont=0;
        char erro;
        int cont_erro = 0;
        int cont_espaco = 0;
        for (i=0; nome_func[i] != '\0'; i++)
        {
            if (cont_espaco > 1)
            {
                break;
            }
            if (nome_func[i] == ' ')
            {
                for (j=i+1; nome_func[j] != ' '; j++)
                {
                    if (nome_func[j] == '\0')
                    {
                        break;
                    }
                    if (!isdigit(nome_func[j]))
                    {
                        erro = nome_func[j];
                        erro_log = -1;
                        arquivo_log(linha, j, funcao, erro, erro_log);
                        return -1;
                    }
                }    
                cont++;            
            }
        }
        if (cont > 1)
        {
            erro_log = 1;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        if (cont < 1)
        {
            erro_log = 1;
            arquivo_log(linha, cont, funcao, erro, erro_log);
            return 2;
        }
        else if (cont == 1)
        {
            return 0;
        }
    }
}    
void arquivo_log(int linha, int j, char *funcao, char erro, int retorno)
{
    FILE *log = fopen("erros.log", "w");
    if (retorno == -1)
    {
        fprintf(log, "A funcao '%s' recebeu um '%c' na coluna '%d' e na linha '%d'. A funcao precisa receber um numero inteiro para funcionar, verifique e tente novamente.\n", funcao, erro, j, linha);
        return;
    }
    if (retorno == 1)
    {
        fprintf(log, "A funcao %s precisa de apenas um argumento, mas recebeu %d na linha '%d'.\n", funcao, j, linha);
        return;
    }
    if (retorno == 0)
    {
        fprintf(log, "A funcao %s precisa de apenas um argumento, mas recebeu %d na linha '%d'.\n", funcao, j, linha);
        return;
    }
    if (retorno == 2)
    {
        fprintf(log, "A funcao %s aceita apenas um numero negativo, mas recebeu %d na linha '%d'.\n", funcao, j, linha);
        return;
    }
    if (retorno == 3)
    {
        fprintf(log, "A funcao %s precisa de um numero float de inicio, apenas pontos sao permitidos entre os numeros, porem na coluna '%d' e linha '%d' a funcao recebeu um %c", funcao, j, linha, erro);
        return;
    }
    if (retorno == 4)
    {
        fprintf(log, "A funcao '%s' aceita apenas um ponto entre numeros, mas recebeu mais de um na coluna '%d' e na linha '%d'.\n", funcao, j, linha);
        return;
    }
    if (retorno == 5)
    {
        fprintf(log, "A funcao '%s' recebeu um '%c' na coluna '%d'e na linha '%d'. A funcao como segundo parametro precisa receber um numero inteiro para funcionar, verifique e tente novamente.\n", funcao, erro, j, linha);
        return;
    }
    if (retorno == 6)
    {
        fprintf(log, "A funcao '%s' so pode receber dois paramentros, mas recebeu %d na linha '%d'\nVerifique e tente novamente.\n", funcao, j, linha);
        return;
    }
    if (retorno == 7)
    {
        fprintf(log, "A funcao '%s' precisa receber dois paramentros, mas recebeu apenas %d na linha '%d'\nVerifique e tente novamente.\n", funcao, j, linha);
        return;
    }
    if (retorno == 8)
    {
        fprintf(log, "A funcao '%s' precisa de 2 argumentos, mas recebeu %d na linha '%d'.\n", funcao, j, linha);
        return;
    }
}

void apaga_lista(gancho *cabeca)
{
    no *atual = cabeca->primeiro;
    no *anterior = NULL;

    while (atual != NULL)
    {
        anterior = atual;
        atual = atual->proximo;
        free(anterior);
    } 
}

