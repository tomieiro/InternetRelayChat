#include <iostream>
#include <string>
#include "cliente.h"

SOCKET self_socket;
int QUIT = 0;
char *aux;

using namespace std;


//Metodo que lanca um erro e termina o programa
//args: (const char*) Frase de erro
void erro(const char erro[100]){
    perror(erro);
    exit(EXIT_FAILURE);
}

//Funcao para matar corretamente o programa fechando descritor da socket
void die_corretly(int signal){
    close(self_socket);
    free(aux);
    printf("\nSaindo...\n");
    exit(EXIT_SUCCESS);
}

void *envia_mensagem(void *args){
    char mensagem[TAM_MSG_MAX];
    string str;
    aux =(char*) malloc(TAM_MAX_BUFFER*sizeof(char));
    int count;
    while(1){
		count = 0;
        printf("VOCE: ");
        getline(cin, str);
        strcpy(aux,str.c_str());
        if(!strcmp(aux, "/quit")) QUIT = 1;
		while(1){
			if((strlen(aux) - count) > TAM_MSG_MAX - 1){
				strncpy(mensagem,&aux[count],TAM_MSG_MAX - 1);
				mensagem[TAM_MSG_MAX - 1] = '\0';
				count += TAM_MSG_MAX;
				send(self_socket, mensagem, TAM_MSG_MAX, 0);
			}else{
				strncpy(mensagem,&aux[count],TAM_MSG_MAX - 1);
				mensagem[TAM_MSG_MAX - 1] = '\0';
				send(self_socket, mensagem, TAM_MSG_MAX, 0);
				break;
			}
		}
    }
}

void *recebe_mensagem(void *args){
	char mensagem[TAM_MSG_MAX];
	while(1){
		if(recv(self_socket, mensagem, TAM_MSG_MAX, 0) == 0) erro("Erro ao receber do servidor!\n");
		printf("\r%s\n", mensagem);
        fflush(stdout);
	}
}


int main(int argc, char *argv[]){
	signal(SIGINT,die_corretly);
	char ip[20]; //Endereco de IP do servidor
    printf("Digite o endereço do servidor (Digite 0.0.0.0 para local): ");
    scanf("%s", ip);
    getchar();

    //Criando Socket com a socket()
	if((self_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) erro("Criacao do Socket falhou!\n");
    
    struct sockaddr_in endereco_servidor;
    
    //Definindo parametros do endereco
	endereco_servidor.sin_family = PF_INET; //Definindo familia do endereco de internet
    endereco_servidor.sin_addr.s_addr = inet_addr(ip);
    endereco_servidor.sin_port = htons(PORTA); //Definindo porta
    
    //Realiza conexao com o servidor
    if(connect(self_socket, (struct sockaddr *)&(endereco_servidor), sizeof(endereco_servidor)) < 0) erro("Criacao de conexao falhou!\n");	
    
    pthread_t enviaMsg;
    if(pthread_create(&enviaMsg, NULL, envia_mensagem, NULL) != 0) erro("Erro ao criar thread de envio!");
    
    pthread_t recebeMsg;
    if(pthread_create(&recebeMsg, NULL, recebe_mensagem, NULL) != 0) erro("Erro ao criar thread de envio!");
    
    while(1){
		//Rodando ate encontrar o SIGINT(Ctrl + C)
        if(QUIT) break;
	}
	close(self_socket);
    free(aux);
	return 0;
}
