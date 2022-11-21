#ifndef FUNCOES_H
#define FUNCOES_H

typedef struct no no;
typedef struct gancho gancho;


gancho* criar_lista(void);
int Inserir_lista(gancho *cabeca, char *vet, int linha);
void printar_lista(gancho *cabeca);
int verifica_func(char *nome_func);
void remove_enter(char *str);
int verifica_args(char *nome_func, int linha);
void arquivo_log(int linha, int j, char *funcao, char erro, int retorno);
void apaga_lista(gancho *cabeca);
int funcao_read(char *nome_func, char *funcao, int linha);
int funcao_storeconst(char *nome_func, char *funcao, int linha);
int funcao_store(char *nome_func, char *funcao, int linha);
int funcao_write(char *nome_func, char *funcao, int linha);
int funcao_jump(char *nome_func, char *funcao, int linha);
int funcao_add(char *nome_func, char *funcao, int linha);
int funcao_sub(char *nome_func, char *funcao, int linha);
#endif