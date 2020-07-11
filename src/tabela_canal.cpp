#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "tabela_canal.h"

#define MASK "255.255.0.0" // Máscara /16 classe B

using namespace std;

// Função que disponibiliza as subredes que podem ser utilizadas 
char** calculate_subnets(char* ip){
    
    char init_subnet[20];
    char subnets[256][20];
    strcpy(init_subnet, "");
    char sub[4];
  
    string aux_ip, aux_mask;
    aux_ip.append(ip);
    aux_mask.append(MASK);
    aux_ip = aux_ip.substr(0, aux_ip.find("."));
    aux_mask = aux_mask.substr(0, aux_ip.find("."));

    for(int i = 0; i < 4; i++){    
        sprintf(sub[i], "%s", "sss");
        strcat(init_subnet,sub[i]);
    }

    // 1st subnet - range: x.x.0.0 - x.x.0.255
    // 256th subnet - range: x.x.255.0 - x.x.255.255

    int atual = (int) init_subnet;

    for(int k = 0; k < 256; k++){
        strcpy(subnets[k],atoi(atual));
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

int hash(char* nome,int tam){

    int value;

    for(int i = 0; nome[i] != '\0'; i++){
        value += (int) nome[i];
    }

    return value%tam;
}

void inserir_canal(CANAL* tab, int tam, char* ip, char* admin_ip, char* nome){

    int ind, k = 0;

    do{ 
        ind = hash(nome,tam) + k;
        k++;
    }while(strcmp(tab[ind].ip,NULL) != 0);

    strcpy(tab[ind].admin_ip, admin_ip);
    strcpy(tab[ind].ip, ip);
    strcpy(tab[ind].nome, nome); 
}

char* verifica_canal(CANAL* tab, char* nome, int tam){

    int ind = hash(nome, tam);

    if(strcmp(tab[ind].ip,NULL) == 0) return NULL;
    return tab[ind].ip;
}


char* busca_canal(CANAL* tab, char* nome, char* user_ip, int tam){

    char* ip = verifica_canal(tab, nome, tam);

    if(!ip) {

        inserir_canal(tab, tam, ip, user_ip, tam);

    }

    return ip;

}