#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <string.h>
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h>

using namespace std;

//DEFININDO CONSTANTES
#define PORTA 31010


//Funcao que lanca uma mensagem de erro e termina o programa
//args: (string) Mensagem de erro
void erro(string erro){
    cerr << erro;
    exit(EXIT_FAILURE);
}

void limpa_buffer(char *buffer){
    for(int i=0; i<4096; i++){
        buffer[i] = 0;
    }
}


int main(int argc, char *argv[]){
    char buffer[4096] = "Tomieiro, vai tomar no..."; //Mensagem 
    int socket_cliente; //Declarando descritor do socket do cliente
    struct sockaddr_in socket_c_address; //Declarando endereco do socket


    //Criando Socket com a socket()
	if((socket_cliente = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!\n");
	
    //Definindo parametros do endereco
    socket_c_address.sin_family = AF_INET; //Definindo familia do endereco de internet
    socket_c_address.sin_addr.s_addr = inet_addr("201.13.159.19"); //Habilitando qualquer interface de conexao
    socket_c_address.sin_port = htons(PORTA); //Definindo porta
	
	//Realiza conexao com o servidor
	if(connect(socket_cliente, reinterpret_cast <struct sockaddr *> (&socket_c_address), sizeof(socket_c_address)) < 0) erro("Criacao de conexao falhou!\n");	
	
	//Manda mensagem para o servidor
    while(1){
       
        scanf("%[^\n]", buffer);
    
        send(socket_cliente, buffer, strlen(buffer), 0);  
        if(buffer[0] != 0){
            limpa_buffer(buffer);
            connect(socket_cliente, reinterpret_cast <struct sockaddr *> (&socket_c_address), sizeof(socket_c_address));
         }
    }
    close(socket_cliente);

	//Fim do programa
	return EXIT_SUCCESS;
}