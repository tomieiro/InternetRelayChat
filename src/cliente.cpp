#include <iostream>
#include <cstdlib>
#include <string>
#include <sys/socket.h> 
#include <netinet/in.h>
 
using namespace std;


//Funcao que lanca uma mensagem de erro e termina o programa
//args: (string) Mensagem de erro
void erro(string erro){
    cerr << erro;
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]){
	
    int socket_cliente; //Declarando descritor do socket do cliente
    struct sockaddr_in socket_c_address; //Declarando endereco do socket
	
    //Criando Socket com a socket()
	if((socket_cliente = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!");
	
    //Definindo parametros do endereco
    socket_c_address.sin_family = AF_INET; //Definindo familia do endereco de internet
    socket_c_address.sin_addr.s_addr = htonl(INADDR_ANY); //Habilitando qualquer interface de conexao
    socket_c_address.sin_port = htons(PORTA); //Definindo porta
	
	if(connect(clientSock, reinterpret_cast <struct sockaddr *> (&socket_c_address), sizeof(socket_c_address)) < 0) erro("Criacao de conexao falhou!";
	
	
	
	
	//Fim do programa
	return EXIT_SUCESS;
}