#ifndef FUNCOES_H
#define FUNCOES_H

typedef struct no no;
typedef struct gancho gancho;


gancho* criar_lista(void);
void inserir_lista(gancho *cabeca, char *str, int col);
int eh_nulo(FILE *arquivo, FILE *arquivo_pronto, FILE *erro_log);
void inserir_arquivo(FILE *arquivo, gancho *cabeca);
void arruma_arquivo(gancho *cabeca);
void deleta_lista(gancho *cabeca);
int verifica_nomes(gancho *cabeca, FILE *erro_log);
no *so_espaco(no *atual, gancho *cabeca);
int retorna_nome(char *funcao);
void remove_enter(char *str);
void remove_espaco(no *aux);
no *remove_item(no *aux, gancho *cabeca);
void tem_hashtag(no *aux);
no *final(gancho *cabeca);
#endif