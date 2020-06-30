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

int hash(char* nome,int tam){

    int value;

    for(int i = 0; nome[i] != '\0'; i++){
        value += (int) nome[i];
    }

    return value%tam;
}

void inserir_canal(CANAL* tab,int tam,char* ip, char* nome){

    int ind, k = 0;

    do{
        
        ind = hash(nome,tam) + k;
        k++;
    }while(strcmp(tab[ind].ip,NULL) != 0);

    strcpy(tab[ind].ip, ip);
    strcpy(tab[ind].nome, nome); 
}


char* verifica_canal(CANAL* tab, char* nome, int tam){

    int ind = hash(nome, tam);

    if(tab[ind].ip == NULL) return NULL;

    return tab[ind].ip;
}

char* busca_canal(CANAL* tab, char* nome, int tam){

    char* ip = verifica_canal(tab, nome, tam);

    if(!ip) {

        inserir_canal(tab, ip,tam, tam); 

        return;
    }


    return ip;

}


