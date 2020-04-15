#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include "socket_utils.h"

using namespace std;

int main(int argc, char *argv[]){

    string buffer; //Buffer
    char mensagem[4096]; //Mensagem
    char ip[20]; //Endereco de IP do servidor

    printf("Digite o endereço do servidor:");
    scanf("%[^\n]%*c", ip);
		
    printf("Digite uma mensagem:");
    getline(cin, buffer);
    
    conexao_cliente Conexao;
    Conexao.cria_conexao(ip);

    //Verifica mensagens maiores que 4096 caracteres e as separam
    do{
        strncpy(mensagem, buffer.c_str(), 4096);
        if(buffer.length() > 4096){
            buffer = buffer.substr(4095, buffer.length()-4095);
            mensagem[4095] = '\0';
            //Manda mensagem para o servidor
            Conexao.envia_mensagem(mensagem);
        }else{
            mensagem[buffer.length()-1] = '\0';
            //Manda mensagem para o servidor
            Conexao.envia_mensagem(mensagem);
            break;
        }
        Conexao.finaliza_conexao();
        Conexao.restart_conexao();
        
    }while(1);
    
    Conexao.finaliza_conexao();

	//Fim do programa
	return EXIT_SUCCESS;
}