#ifndef LISTA_H
#define LISTA_H

#define ERRO -404
#define TIPO int  //Customização do tipo de dados

typedef struct lista LISTA_CANAL;
typedef struct canal CANAL;

/*Funções básicas:*/
LISTA_CANAL *lista_criar(void);
void lista_apagar(LISTA_CANAL *L);

/*Funções de inserção, busca e remoção:*/
TIPO lista_buscar_item(LISTA_CANAL *L, TIPO chave);
//TIPO lista_buscar_recursivo(LISTA *L, TIPO chave);
int lista_inserir_inicio(LISTA_CANAL *L, TIPO item);
int lista_inserir_fim(LISTA_CANAL *L, TIPO item); /*INSERE NA ÚLTIMA POSIÇÃO*/
int lista_remover_item(LISTA_CANAL *L, TIPO chave);

/*Outras funções:*/
//void lista_imprimir(LISTA *L);
int lista_vazia(LISTA_CANAL *L);
int lista_tamanho(LISTA_CANAL *L);

#endif