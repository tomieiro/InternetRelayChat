#ifndef LISTA_CANAL_H
#define LISTA_CANAL_H

#include "lista_clientes.h"

#define ERRO -404

typedef struct listacanal LISTA_CANAL;
typedef struct canal CANAL;

struct listacanal{
	CANAL *inicio;
	CANAL *fim;
	int tam;
};

struct canal{
	//O admin é sempre o nó raíz
	LISTA *clientes;
	char nome_canal[200];
	CANAL *proximo;
};

/*Funções básicas:*/
LISTA_CANAL *lista_canais_criar(void);
void lista_canais_apagar(LISTA_CANAL *L);

/*Funções de inserção, busca e remoção:*/
CANAL *lista_canais_buscar_item(LISTA_CANAL *L, char chave[200]);
//TIPO lista_buscar_recursivo(LISTA *L, TIPO chave);
int lista_canais_inserir(LISTA_CANAL *L, CANAL *C); /*INSERE NA ÚLTIMA POSIÇÃO*/
//int lista_canal_remover_item(LISTA_CANAL *L, char chave[200]);

/*Outras funções:*/
//void lista_imprimir(LISTA *L);
int lista_canais_vazia(LISTA_CANAL *L);
int lista_canais_tamanho(LISTA_CANAL *L);
int verificar_admin(CANAL *canal, NO *usuario);

#endif