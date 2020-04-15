#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

//DEFININDO CONSTANTES
#define PORTA 31010
#define TAM_FILA_REQUERENTES 3

//Declarando descritor dos sockets de servidor e cliente
int socket_servidor, socket_cliente;

//Funcao que lanca uma mensagem de erro e termina o programa
//args: (string) Mensagem de erro
void erro(string erro){
    cerr << erro;
    exit(EXIT_FAILURE);
}

void limpa_buffer(char *buffer){
    for(int i=0; i<4096; i++) buffer[i] = 0;
}

void finalizar(int signum){
    close(socket_servidor);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
    signal(SIGINT, finalizar);
    struct sockaddr_in socket_s_address, socket_c_address; //Declarando enderecos dos sockets(sao structs predefinidas la no socket.h)
	char buffer[4096] = {0};
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

    //Criando vetor de enderecos
    struct sockaddr **enderecos = (struct sockaddr**)malloc(sizeof(struct sockaddr *)*2);

	//Esperando conexao com cliente
    while(true){
		if((socket_cliente = accept(socket_servidor, reinterpret_cast <struct sockaddr *> (&socket_c_address), &tam_cliente_address))<0) erro("Falha ao aceitar conexoes!\n");
    	if((tam_mensagem = recv(socket_cliente, buffer, 4096, 0)) < 0) erro("Falha ao receber mensagem!\n");
        printf("MENSAGEM: %s\n",buffer);
        if(buffer[0] != 0){
            limpa_buffer(buffer);
            printf("TAM: %d\n\n",tam_mensagem);
        }
	}
}