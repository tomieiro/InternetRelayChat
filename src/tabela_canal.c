#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_canal.h"

#define MASK "255.255.0.0"

void create_subnet(char* ip){

    char* mask;
    strcpy(mask,MASK);
    char* subnet[32] = "";
    int sub[4], i = 0;
    char* aux;
    do{
       
       sub[i] = (int) strtok(ip, ".") & (int) strtok(mask, ".");

        i++;
    }while(i < 3);

    for(int i = 0; i < 4; i++){
        strcat(subnet,sub[i]);
    }

    

}

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

    int ind = hash(nome, tam);


    if(strcmp(tab[ind].ip, NULL) == 0) {
        
    
    }

    return tab[ind].ip;

}




