#ifndef FUNCOES_H
#define FUNCOES_H

typedef struct no no;
typedef struct gancho gancho;


gancho* criar_lista(void);
void inserir_lista(gancho *cabeca, char *str, int col);
int eh_nulo(FILE *arquivo, FILE *erro_log);
void inserir_arquivo(FILE *arquivo, gancho *cabeca);
void arruma_arquivo(gancho *cabeca);
void deleta_lista(gancho *cabeca);
int verifica_nomes(gancho *cabeca, FILE *erro_log);
no *so_espaco(no *atual, gancho *cabeca);
int retorna_nome(no *aux);
int um_paramentro(no *aux, FILE *arquivo, FILE *erro_log);
int dois_paramentros(no *aux, FILE *arquivo, FILE *erro_log);
int storeconst(no *aux, FILE *arquivo, FILE *erro_log);
void remove_enter(char *str);
void remove_espaco(no *aux);
no *remove_item(no *aux, gancho *cabeca);
void tem_hashtag(no *aux);
int verifica_args(gancho *cabeca, FILE *arquivo, FILE *erro_log);
no *final(gancho *cabeca);
char *nome_func(no *aux);
int args_espaco(no *aux, int n);
#endif