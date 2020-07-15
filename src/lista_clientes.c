#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_clientes.h"

//Funcao que cria uma lista
//return (LISTA*) lista criada
LISTA *lista_criar(){
	LISTA *newl = (LISTA *)malloc(sizeof(LISTA));
	if(newl != NULL){		
		newl->inicio = NULL;
		newl->fim = NULL;
		newl->tam = 0;
	}
	return newl;
}
//Funcao que apaga uma lista
//args: (LISTA*) lista qual se deseja alterar
void lista_apagar(LISTA *L){
	if(L != NULL && !lista_vazia(L)){
   		NO *no = L->inicio,*aux;
    	while(no != L->fim){
       	 	aux = no;
       	 	no = no->proximo;
        	free(aux);
    	}
    	free(L->fim);
    	L->inicio = NULL;
    	L->fim = NULL;
		free(L);
		L = NULL;
	}
}

//Funcao que busca um item em uma lista
//args: (LISTA*) lista qual se deseja alterar, (char*) string do ip
//return:(SOCKET) socket relacionado aquele IP
SOCKET lista_buscar_item(LISTA *L, char ip[20]){
	NO *aux = L->inicio;
	if(L != NULL){
		while(aux != NULL){
			if(strcmp(aux->ip,ip)) return aux->self_socket;
			aux = aux->proximo;
		}
	}
	return (ERRO);
}	

//Funcao que insere um item em uma lista
//args: (LISTA*) lista qual se deseja alterar, (char*)string do ip, (SOCKET)FD do socket a inserir
//return:(int) 1 se sucesso, 0 se fracasso
int lista_inserir(LISTA *L, char ip[20], SOCKET self_socket){
	NO *aux = NULL;
	if(L == NULL) return (ERRO);
	aux = (NO *) malloc(sizeof(NO));
	if(aux != NULL){
        strcpy(aux->ip,ip);
        aux->self_socket = self_socket;
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

//Funcao que remove um item de uma lista
//args: (LISTA*) lista qual se deseja alterar, (char*)IP qual se deseja remover o FD
//return: (int) 1 se o item foi removido com sucesso, 0 se houve erro
int lista_remover_item(LISTA *L, char ip[20]){
	NO *p = NULL, *aux = NULL;
	if(L != NULL && !lista_vazia(L)){
		p = L->inicio;
		while((p != NULL) && (!strcmp(p->ip,ip))){
			aux = p; /* aux_busca recebe o nó anterior de aux_remocao*/
			p = p->proximo;
		}
		if(p != NULL){
			if(p == L->inicio){ /*Exceção: chave no inicio*/
				L->inicio = p->proximo;
				p->proximo = NULL;
			}else{
				aux->proximo = p->proximo;
				p->proximo = NULL;
			}
			if(p == L->fim){ /* Se a chave está no último nó*/
				L->fim = aux;
			}
			L->tam--;
			free(p);	
			return 1;
		}
	}
	return 0;
}

//Funcao que verifica se uma lista esta vazia
//args: (LISTA*) lista qual se deseja verificar
//return:(int) 0 se esta cheia, 1 se esta vazia
int lista_vazia(LISTA *L){
	if(L != NULL && L->inicio == NULL) return 1;
 	return 0;
}

//Funcao que printa uma lista
//args: (LISTA*) lista qual se deseja printar
void printLista(LISTA *l){
	NO *aux = l->inicio;
	while(aux != NULL){
			printf("FD: %d ; IP: %s\n",aux->self_socket, aux->ip);
			aux = aux->proximo;
		}
}