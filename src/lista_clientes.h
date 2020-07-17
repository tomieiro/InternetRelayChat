#ifndef LISTA_H
#define LISTA_H

#define ERRO -404
#define TIPO int
#define SOCKET int

typedef struct LISTA_ LISTA;
typedef struct NO_ NO;

struct LISTA_{
	NO *inicio;
	NO *fim;
	int tam;
};

struct NO_{
	NO *proximo;
    char usuario[50];
    char ip[20];
    SOCKET self_socket;
	int mutado;
};

/*Funções básicas:*/
LISTA *lista_criar();
void lista_apagar(LISTA *L);

/*Funções de inserção, busca e remoção:*/
SOCKET lista_buscar_item(LISTA *L, char *ip);
SOCKET lista_buscar_item_por_user(LISTA *L, char *username);
char *lista_buscar_ip(LISTA *L, char *username);
NO *lista_buscar_cliente(LISTA *L, char *username);
int user_exists(LISTA *L, char username[50]);

int lista_inserir(LISTA *L, char ip[20], SOCKET self_socket, char username[50], int mutado);
int lista_remover_item(LISTA *L, char *ip);
int lista_remover_item_por_user(LISTA *L, char *username);

/*Outras funções:*/
int lista_vazia(LISTA *L);
void printLista(LISTA *l);

#endif