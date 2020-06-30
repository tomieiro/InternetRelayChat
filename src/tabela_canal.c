#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_canal.h"

CANAL* criar_tabela(int tam){

    CANAL tabela[tam];

    for(int i = 0; i < tam; i++){
        strcpy(tabela[i].ip,NULL);
        strcpy(tabela[i].nome,NULL);
    }

    return tabela;
}

int hash(int value,int tam){

    return value%tam;
}

void inserir_canal(CANAL* tab,int tam,char ip[20], char* nome){

    int ind = 0;

    while(strcmp(tab[ind].ip,NULL)){
        
        ind = hash((int)nome,tam);
     
    }

    strcpy(tab[ind].ip, ip);
    strcpy(tab[ind].nome, nome); 
}

char* busca_hash(CANAL* tab, char* nome, int tam){

    int ind = hash((int) nome, tam);

    if(strcmp(tab[ind].ip, NULL) == 0) {
        return NULL;
    }

    return tab[ind].ip;

}




