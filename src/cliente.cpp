#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <sys/socket.h> 
#include <netinet/in.h>

using namespace std;

//DEFININDO CONSTANTES
#define PORTA 8080


//Funcao que lanca uma mensagem de erro e termina o programa
//args: (string) Mensagem de erro
void erro(string erro){
    cerr << erro;
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]){
    char buffer[4096]; //Mensagem 
    int socket_cliente; //Declarando descritor do socket do cliente
    struct sockaddr_in socket_c_address; //Declarando endereco do socket

    scanf("%s\n",buffer);

    //Criando Socket com a socket()
	if((socket_cliente = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!\n");
	
    //Definindo parametros do endereco
    socket_c_address.sin_family = AF_INET; //Definindo familia do endereco de internet
    socket_c_address.sin_addr.s_addr = htonl(INADDR_ANY); //Habilitando qualquer interface de conexao
    socket_c_address.sin_port = htons(PORTA); //Definindo porta
	
	//Realiza conexao com o servidor
	if(connect(socket_cliente, reinterpret_cast <struct sockaddr *> (&socket_c_address), sizeof(socket_c_address)) < 0) erro("Criacao de conexao falhou!\n");	
	
	//Manda mensagem para o servidor
	send(socket_cliente, buffer, strlen(buffer), 0);
	
	//Fim do programa
	return EXIT_SUCCESS;
}