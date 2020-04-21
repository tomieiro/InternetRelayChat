#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include "socket_utils.h"

using namespace std;

int main(int argc, char *argv[]){

    string buffer, user;//Buffer
    char mensagem[4096]; //Mensagem
    char ip[20]; //Endereco de IP do servidor

    printf("Digite seu nome de usuário?\n");
    getline(cin, user);
    user = user + ": ";

    printf("Digite o endereço do servidor (Digite 0.0.0.0 para local): ");
    scanf("%[^\n]s", ip);
    //if(strcmp(ip, "\n")) strcpy(ip, "0.0.0.0");
	
    getchar();
    
    conexao_cliente Conexao;
    Conexao.cria_conexao(ip);

    //Verifica mensagens maiores que 4096 caracteres e as separam
    while(1){
        printf("Digite uma mensagem: ");
        getline(cin, buffer);
        if(!strcmp(buffer.c_str(), "/quit")) break;
        do{
            buffer = user + buffer;
            strncpy(mensagem, buffer.c_str(), 4096);
            if(buffer.length() > 4096){
                buffer = buffer.substr(4095, buffer.length()-4095);
                mensagem[4095] = '\0';
                //Manda mensagem para o servidor
                Conexao.set_mensagem(mensagem);
                Conexao.envia_mensagem();
                Conexao.finaliza_conexao();
                Conexao.restart_conexao();
                //printf("%s\n", Conexao.recebe_mensagens());
            }else{
                mensagem[buffer.length()] = '\0';
                //Manda mensagem para o servidor
                Conexao.set_mensagem(mensagem);
                Conexao.envia_mensagem();
                Conexao.finaliza_conexao();
                Conexao.restart_conexao();
                //printf("%s\n", Conexao.recebe_mensagens());
                break;
            }
        }while(1);
    }
    Conexao.finaliza_conexao();

	//Fim do programa
	return EXIT_SUCCESS;
}