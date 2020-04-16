#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include "socket_utils.h"

using namespace std;

int main(int argc, char *argv[]){

    string buffer; //Buffer
    char mensagem[4096]; //Mensagem
    string nome_usuario; //Nome do usuário
    char ip[20]; //Endereco de IP do servidor
    int tamanho_real_mensagem;

    printf("Escreva um nome para o usuário: ");
    getline(cin, nome_usuario);
    tamanho_real_mensagem = 4096 - nome_usuario.length(); 
    strncpy(mensagem, nome_usuario.c_str(), nome_usuario.length()); 

    printf("Digite o endereço do servidor (Digite 0.0.0.0 para local): ");
    scanf("%[^\n]s", ip);
	
    getchar();
    
    conexao_cliente Conexao;
    Conexao.cria_conexao(ip);

    //Verifica mensagens maiores que 4096 caracteres e as separam
    while(1){
        printf("Digite uma mensagem: ");
        getline(cin, buffer);
              
        if(!strcmp(buffer.c_str(), "/quit")) break;
        do{
            strncpy(mensagem+nome_usuario.length(), buffer.c_str(), tamanho_real_mensagem);
            if(buffer.length() > tamanho_real_mensagem){
                buffer = buffer.substr(tamanho_real_mensagem-1, buffer.length()-(tamanho_real_mensagem-1));
                mensagem[4095] = '\0';
                //Manda mensagem para o servidor
                Conexao.set_mensagem(mensagem);
                Conexao.envia_mensagem();
                Conexao.finaliza_conexao();
                Conexao.restart_conexao();
            }else{
                mensagem[buffer.length()+nome_usuario.length()] = '\0';
                //Manda mensagem para o servidor
                Conexao.set_mensagem(mensagem);
                Conexao.envia_mensagem();
                Conexao.finaliza_conexao();
                Conexao.restart_conexao();
                break;
            }
        }while(1);
    }
    Conexao.finaliza_conexao();

	//Fim do programa
	return EXIT_SUCCESS;
}