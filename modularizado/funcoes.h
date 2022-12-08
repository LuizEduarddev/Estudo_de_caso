#ifndef FUNCOES_H
#define FUNCOES_H

typedef struct no no;
typedef struct gancho gancho;


gancho* criar_lista(void);
void inserir_lista(gancho *cabeca, char *str, int col);
int eh_nulo(FILE *arquivo, FILE *erro_log);
void inserir_arquivo(FILE *arquivo, gancho *cabeca);
void arruma_arquivo(gancho *cabeca, FILE *erro_log);
void deleta_lista(gancho *cabeca);
int verifica_nomes(gancho *cabeca, FILE *erro_log);
no *so_espaco(no *atual, gancho *cabeca);
int retorna_nome(char *funcao);
int um_paramentro(no *aux, FILE *arquivo, FILE *erro_log, int *vetor, int *store);
int dois_parametros(no *aux, FILE *arquivo, FILE *erro_log, int *vetor, int *store);
int storeconst(no *aux, FILE *arquivo, FILE *erro_log, int *vetor, int *store);
int jump(no *aux, FILE *arquivo, FILE *erro_log, gancho *cabeca);
void remove_enter(char *str);
void remove_espaco(no *aux);
no *remove_item(no *aux, gancho *cabeca);
void tem_hashtag(no *aux);
int verifica_args(gancho *cabeca, FILE *arquivo, FILE *erro_log);
no *final(gancho *cabeca);
char *nome_func(no *aux);
int confere_jump(int col, FILE *arquivo, FILE *erro_log, int jump_pula, int maior);
int pega_parametro(no *aux, FILE *erro_log);
int tira_nome(no *aux);
void inserir_parametro(no *aux, char *str, int cont);
int n_tipo(char *str);
int para_jump(char *str);
void realiza_func(no *aux, char *nome, int *vetor, int *store);
void funcao_read(no *aux, int *vetor);
void funcao_storeconst(no *aux, int *vetor);
void funcao_store(no *aux, int *vetor, int *store);
void funcao_jump(no *aux, int *vetor);
int funcao_sub(no *aux, int *vetor);
int funcao_div(no *aux, int *vetor);
int funcao_add(no *aux, int *vetor);
int funcao_mult(no *aux, int *vetor);
void funcao_write(no *aux, int *vetor);
#endif