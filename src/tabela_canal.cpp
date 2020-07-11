#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "tabela_canal.h"

#define MASK "255.255.0.0" // Máscara /16 classe B

using namespace std;

//
char** alocar_tabela(int l, int c){

    char **mat = (char**) malloc(l* sizeof(char *));
    int row;

     for(row = 0; row < l; row++){
        mat[row] = (char *) malloc(c* sizeof(char));
     }

    return mat;    

}

//
void libera_tabela(char** tab, int l, int c){

    for(int i = 0; i < l; i++){
        free(tab[i]);
    }
    free(tab);
}

// Função que disponibiliza as subredes que podem ser utilizadas 
char** calculate_subnets(char* ip){
    char init_subnet[20];
    char** subnets = alocar_tabela(256,20);
    strcpy(init_subnet, "");
    int sub[4];
    int k = 0, pos = 0, aux_pos;
    string aux_ip, aux_mask, or_ip, mask, aux;
    or_ip.append(ip);
    mask.append(MASK);
    
    
    for(int i = 0; i < 4; i++){    

        aux_pos = or_ip.find(".", pos+1);
        if(pos != 0) pos++;
        aux_ip = or_ip.substr(pos, aux_pos - pos);
        aux_mask = mask.substr(pos, aux_pos - pos);
        sub[i] =  atoi(aux_ip.c_str()) & atoi(aux_mask.c_str());
        aux = to_string(sub[i]);
        strcat(init_subnet, aux.c_str());
        pos = aux_pos;
    }

    
    // 1st subnet - range: x.x.0.0 - x.x.0.255
    // 256th subnet - range: x.x.255.0 - x.x.255.255
 
    int atual = atoi(init_subnet);

    for(int k = 0; k < 256; k++){
        aux = to_string(atual);
        strcpy(subnets[k], aux.c_str());
        atual = atual  + 1000;
    }

    return subnets;

}

CANAL* criar_tabela(int tam){

    CANAL tabela[tam];

    for(int i = 0; i < tam; i++){
        strcpy(tabela[i].ip,NULL);
        strcpy(tabela[i].nome,NULL);
    }

    return tabela;
}

int hash_ind(char* nome,int tam){

    int value;

    for(int i = 0; nome[i] != '\0'; i++){
        value += (int) nome[i];
    }

    return value%tam;
}

void inserir_canal(CANAL* tab, int tam, char* ip, char* admin_ip, char* nome){


    int ind, k = 0;

    do{ 
        ind = hash_ind(nome,tam) + k;
        k++;
    }while(strcmp(tab[ind].ip,NULL) != 0);

    strcpy(tab[ind].admin_ip, admin_ip);
    strcpy(tab[ind].ip, ip);
    strcpy(tab[ind].nome, nome); 
}

char* verifica_canal(CANAL* tab, char* nome, int tam){

    int ind = hash_ind(nome, tam);

    if(strcmp(tab[ind].ip,NULL) == 0) return NULL;
    return tab[ind].ip;
}


char* busca_canal(CANAL* tab, char* nome, char* user_ip, int tam){

    char* ip = verifica_canal(tab, nome, tam);

    if(!ip) {
        inserir_canal(tab, tam, ip, user_ip,nome); 
    }

    return ip;

}