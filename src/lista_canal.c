#include <stdio.h>
#include <stdlib.h>
#include "lista_canal.h"
#include "lista_clientes.h"


LISTA_CANAL *criar_lista_canais(void){
	LISTA_CANAL *novo = (LISTA_CANAL *)malloc(sizeof(LISTA_CANAL));
	if(novo != NULL){		
		novo->inicio = NULL;
		novo->fim = NULL;
		novo->tam = 0;
	}
	return novo;
}

int lista_canal_inserir(LISTA_CANAL *L, CANAL *C){
	CANAL *aux = NULL;
	if(L == NULL) return (ERRO);
	aux = (CANAL *) malloc(sizeof(CANAL));
	if(aux != NULL){
		aux->clientes = C->clientes;
		strcpy(aux->nome_canal,C->nome_canal);
		aux->proximo = NULL;
		/*CASO DE PRIMEIRO ELEMENTO*/
		if(lista_vazia(L)){
			L->inicio = aux;
			L->fim = aux;
			L->tam++;
		}else{ /*CASO DE QUALQUER ELEMENTO QUE NÃO O PRIMEIRO*/
			L->fim->proximo = aux;
			L->fim = aux;
			L->tam++;
		}
		return 1; /*SUCESSO*/
 	}
	return 0; /*FALHA*/
}

void apagar_lista_canais(LISTA_CANAL *L){
	if(L != NULL && !lista_vazia(L)){
   		CANAL *no = L->inicio, *aux;
    	while(no != L->fim){
       	 	aux = no;
       	 	no = no->proximo;
			lista_apagar(no->clientes);
        	free(aux);
    	}
    	free(L->fim);
    	L->inicio = NULL;
    	L->fim = NULL;
		free(L);
		L = NULL;
	}
}

//nome_canal é chave
CANAL *lista_canais_buscar_item(LISTA_CANAL *L, char chave[200]){
	CANAL *aux = L->inicio;
	if(L != NULL){
		while(aux != NULL){
			if(!strcmp(aux->nome_canal,chave)) return aux;
			aux = aux->proximo;
		}
	}
	return NULL;
}	

/*
int lista_canais_remover_item(LISTA_CANAL *L, CANAL chave){
	CANAL *p = NULL, *aux = NULL;
	if(L != NULL && !lista_vazia(L)){
		p = L->inicio;
		while((p != NULL) && (p->item != chave)){
			aux = p; // aux_busca recebe o nó anterior de aux_remocao
			p = p->proximo;
		}
		if(p != NULL){
			if(p == L->inicio){ //Exceção: chave no inicio
				L->inicio = p->proximo;
				p->proximo = NULL;
			}else{
				aux->proximo = p->proximo;
				p->proximo = NULL;
			}
			if(p == L->fim){ // Se a chave está no último nó
				L->fim = aux;
			}
			L->tam--;
			free(p);	
			return 1;
		}
	}
	return 0;
}*/

int lista_canais_vazia(LISTA_CANAL *L){
	if(L != NULL && L->inicio == NULL) return 1;
 	return 0;
}

int lista_canais_tamanho(LISTA_CANAL *L){
	return (L != NULL ? L->tam : ERRO);	
}