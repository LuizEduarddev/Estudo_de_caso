#ifndef FUNCOES_H
#define FUNCOES_H

typedef struct no no;
typedef struct gancho gancho;


gancho* criar_lista(void);
void inserir_lista(gancho *cabeca, char *vet);
int eh_nulo(FILE *arquivo, FILE *arquivo_pronto, FILE *erro_log);
void inserir_arquivo(FILE *arquivo, gancho *cabeca);
void arruma_arquivo(gancho *cabeca, FILE *arquivo_pronto);
void deleta_lista(gancho *cabeca);
int verifica_nomes(FILE *arquivo_pronto);
int so_espaco(no *aux);
void remove_espaco(no *aux);
void inserir_arquivo_pronto(gancho *cabeca, FILE *arquivo_pronto);
#endif