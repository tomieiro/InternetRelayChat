#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

//DEFININDO CONSTANTES
#define PORTA 31010


//Funcao que lanca uma mensagem de erro e termina o programa
//args: (string) Mensagem de erro
void erro(string erro){
    cerr << erro;
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]){

    string buffer; //Buffer
    char mensagem[4096]; //Mensagem
    int socket_cliente; //Declarando descritor do socket do cliente
    struct sockaddr_in socket_c_address; //Declarando endereco do socket
    char serv_addr[100]; //Endereço do servidor

    printf("Digite o endereço do servidor:");
    scanf("%[^\n]%*c", serv_addr);

    //Criando Socket com a socket()
	if((socket_cliente = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
        erro("Criacao do Socket falhou!\n");
	
    //Definindo parametros do endereco
    socket_c_address.sin_family = AF_INET; //Definindo familia do endereco de internet
    //socket_c_address.sin_addr.s_addr = htonl(INADDR_ANY); //Habilitando qualquer interface de conexao
    //socket_c_address.sin_addr.s_addr = inet_addr("201.13.159.19");
    socket_c_address.sin_addr.s_addr = inet_addr(serv_addr);
    socket_c_address.sin_port = htons(PORTA); //Definindo porta
	
	//Realiza conexao com o servidor
	if(connect(socket_cliente, reinterpret_cast <struct sockaddr *> (&socket_c_address), sizeof(socket_c_address)) < 0)
        erro("Criacao de conexao falhou!\n");	
	
    printf("Digite uma mensagem:");
    getline(cin, buffer);
    
    //Verifica mensagens maiores que 4096 caracteres e as separam
    do{
        strncpy(mensagem, buffer.c_str(), 4096);
        if(buffer.length() > 4096){
            buffer = buffer.substr(4095, buffer.length()-4095);
            mensagem[4095] = '\0';
            //Manda mensagem para o servidor
            printf("%lu\n", send(socket_cliente, mensagem, 4096, 0));
            //printf("\n\n%s\n\n", mensagem);
        }else{
            mensagem[buffer.length()-1] = '\0';
            //Manda mensagem para o servidor
            printf("%lu\n", send(socket_cliente, mensagem, 4096, 0));
            //printf("\n\n%s\n\n", mensagem);
            break;
        }
    }while(1);
	
    close(socket_cliente);

	//Fim do programa
	return EXIT_SUCCESS;
}