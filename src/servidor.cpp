#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/socket.h> 
#include <netinet/in.h>
 
using namespace std;

//DEFININDO CONSTANTES
#define PORTA 8080
#define TAM_FILA_REQUERENTES 3


//Funcao que lanca uma mensagem de erro e termina o programa
//args: (string) Mensagem de erro
void erro(string erro){
    cerr << erro;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int socket_servidor, socket_cliente; //Declarando descritor dos sockets de servidor e cliente
    struct sockaddr_in socket_s_address, socket_c_address; //Declarando enderecos dos sockets(sao structs predefinidas la no socket.h)
	char buffer[4096];
    int tam_mensagem;

	//Criando Socket com socket()
    if((socket_servidor = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!\n");

    //Definindo parametros do endereco
    socket_s_address.sin_family = AF_INET; //Definindo familia do endereco de internet
    socket_s_address.sin_addr.s_addr = htonl(INADDR_ANY); //Habilitando qualquer interface de conexao
    socket_s_address.sin_port = htons(PORTA); //Definindo porta

    //Reservando porta para o socket usando o bind()
    if(bind(socket_servidor, reinterpret_cast <struct sockaddr *> (&socket_s_address), sizeof(socket_s_address)) < 0) erro("Reserva de endereco falhou!\n");
   
    //Habilita o socket para receber conexoes
    if(listen(socket_servidor, TAM_FILA_REQUERENTES) < 0) erro("Habilitar  de conexoes falhou!\n");
	
	//Tamanho da struct
	socklen_t tam_cliente_address = sizeof(socket_c_address);
	
	//Esperando conexao com cliente
    while(1){
		if((socket_cliente = accept(socket_servidor, reinterpret_cast <struct sockaddr *> (&socket_c_address), &tam_cliente_address)) < 0) erro("Falha ao aceitar conexoes!\n");
    	if((tam_mensagem = recv(socket_cliente, buffer, 4096, 0)) < 0) erro("Falha ao receber mensagem!\n");
        printf("MENSAGEM: %s\n",buffer);
        send(socket_cliente, buffer, strlen(buffer), 0);
	}
	//Fim do programa
	return EXIT_SUCCESS; 
}