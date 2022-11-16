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
    FILE *log;
    char nome_func[BUFFER];
    int verifica = 0, linha = 1;
    arquivo = fopen("C:\\Users\\dudud\\OneDrive\\Documentos\\Faculdade\\Estrutura de Dados\\Estudo de caso\\arquivo.txt", "r");
    log = fopen("erros.log", "w");
    if (arquivo == NULL || log == NULL)
    {
        printf("Parece que houve um erro ao abrir o arquivo, por favor tente novamente.\n");
        return -1;
    }
    
    while (!feof(arquivo))
    {
        fflush(stdin);
        fgets(nome_func, BUFFER, arquivo);
        verifica = verifica_func(nome_func);
        if (verifica == 1)
        {
            printf("O que foi digitado foi um numero, quando na verdade deveria ser uma string para uma funcao.\nNumero digitado - %s, na linha %d", nome_func, linha);
            return 1;
        }
        if (verifica == -1)
        {
            printf("A funcao digitada esta incorreta na linha %d.\nFuncao digitada - %s\n", nome_func, linha);
            return -1;
        }
        while (verifica == 10)
        {
            fgets(nome_func, BUFFER, arquivo);
            verifica = verifica_func(nome_func);
        }
        verifica = verifica_args(nome_func, linha);
        if (verifica == -1)
        {
            printf("O arquivo retornou um erro.\n");
            return -1;
        }
        if (verifica == 2)
        {
            printf("O arquivo retornou um erro.\n");
            return 2;
        }
        linha = Inserir_lista(cabeca, nome_func, linha);
    }
    apaga_lista(cabeca);
    fprintf(log, "Sem erros para serem mostrados no momento.\n");
    printf("O arquivo nao retornou erros.\n");
}