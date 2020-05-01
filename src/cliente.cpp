#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "socket_utils.h"
#include <pthread.h>

using namespace std;

conexao_cliente Conexao;
bool SAIR = false;

void next_msg(){
    
    Conexao.restart_conexao();
}

void *envia_mensagem(void *args){
    string buffer; //Buffer
    char mensagem[4096]; //Mensagem
    string nome_usuario; //Nome do usuário
    int tamanho_real_mensagem;

    printf("Escreva um nome para o usuário: ");
    getline(cin, nome_usuario);
    nome_usuario = nome_usuario + " : ";
    tamanho_real_mensagem = 4096 - nome_usuario.length(); 
    strncpy(mensagem, nome_usuario.c_str(), nome_usuario.length()); 
    while(true){
        getline(cin, buffer);
        if(!strcmp(buffer.c_str(), "/quit")){
            SAIR = true;
            break;
        }
        do{
            //Verifica mensagens maiores que 4096 caracteres e as separam
            strncpy(mensagem+nome_usuario.length(), buffer.c_str(), tamanho_real_mensagem);
            if(buffer.length() > tamanho_real_mensagem){
                buffer = buffer.substr(tamanho_real_mensagem-1, buffer.length()-(tamanho_real_mensagem-1));
                mensagem[4095] = '\0';
                //Manda mensagem para o servidor
                Conexao.set_mensagem(mensagem);
                Conexao.envia_mensagem();
                next_msg();
            }else{
                mensagem[buffer.length()+nome_usuario.length()] = '\0';
                //Manda mensagem para o servidor
                Conexao.set_mensagem(mensagem);
                Conexao.envia_mensagem();
                next_msg();
                break;
            }
        }while(1);
    }
    return NULL;
}

void *recebe_mensagem(void *args){
    int size = 0;
    while(true){
        size = Conexao.recebe_mensagens();
        if(size > 0){
            printf("\r%s\nDigite sua mensagem: ",Conexao.get_mensagem());
            Conexao.limpa_mensagem();
            fflush(stdout);
            size = 0;
        }
    }
    return NULL;
}


int main(int argc, char *argv[]){
    char ip[20]; //Endereco de IP do servidor
    
    printf("Digite o endereço do servidor (Digite 0.0.0.0 para local): ");
    scanf("%[^\n]s", ip);

    getchar();

    Conexao.cria_conexao(ip);

    pthread_t enviaMensagemC;
    if(pthread_create(&enviaMensagemC, NULL,envia_mensagem, NULL) != 0){
        exit(EXIT_FAILURE);
    }
    pthread_t recebeMensagemC;
    if(pthread_create(&recebeMensagemC, NULL,recebe_mensagem, NULL) != 0){
        exit(EXIT_FAILURE);
    }
    
	//Fim do programa
	while(true){
    if(SAIR)
        return EXIT_SUCCESS;
    }
    Conexao.finaliza_conexao();
}