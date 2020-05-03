#include "servidor.h"

SOCKET self_socket, socket_clientes_atual;
LISTA *clientes;

//Metodo que lanca um erro e termina o programa
//args: (const char*) Frase de erro
void erro(const char erro[100]){
    perror(erro);
    exit(EXIT_FAILURE);
}

//Funcao para matar corretamente o programa fechando descritor da socket
void die_corretly(int signal){
    NO *aux = clientes->inicio;
    while(aux != NULL){
        close(aux->self_socket);
        aux = aux->proximo;
    }
    close(self_socket);
    lista_apagar(clientes);
    printf("\nFechando servidor...\n");
    exit(EXIT_SUCCESS);
}

//Funcao que gerencia todos os clientes no servidor
//args:(NO*) No atual da lista do cliente que se conectou no momento
void gerencia_dados(NO *atual){
    signal(SIGINT,die_corretly);
    NO *aux;
    int recebidos;
    char buffer[TAM_MSG_MAX];
    while(1){
        aux = clientes->inicio;
        recebidos = recv(atual->self_socket, buffer, TAM_MSG_MAX, 0);
        if(recebidos <= 0){
            lista_remover_item(clientes, atual->ip);
            break;
        }
        while(aux != NULL){
            if(aux->self_socket != atual->self_socket){
                send(aux->self_socket, buffer, TAM_MSG_MAX, 0);
            }
			aux = aux->proximo;
		}
    }
}

int main(int argc, char *argv[]){
    //Criando lista
    clientes = lista_criar();
	signal(SIGINT,die_corretly);
    
    //Criando Socket com a socket()
	if((self_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) erro("Criacao do Socket falhou!\n");
    
    struct sockaddr_in endereco_servidor, endereco_cliente;
    
    //Definindo parametros do endereco
	endereco_servidor.sin_family = PF_INET; //Definindo familia do endereco de internet
    endereco_servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    endereco_servidor.sin_port = htons(PORTA); //Definindo porta
    
    //Reservando porta para o socket usando o bind()
    if(bind(self_socket, (struct sockaddr *)&(endereco_servidor), sizeof(endereco_servidor)) < 0) erro("Reserva de endereco falhou!\n");
   
    //Habilita o socket para receber conexoes
    if(listen(self_socket, MAX_CLIENTES) < 0) erro("Habilitar  de conexoes falhou!\n");	
    
    socklen_t aux = sizeof(endereco_cliente);

    while(1){//Rodando ate que CTRL + C seja usado
        //Aceitando conexoes dos clientes e abrindo uma thread para cada um
		socket_clientes_atual = accept(self_socket, (struct sockaddr*)&endereco_cliente, (socklen_t*)&aux);
        printf("O IP: %s se conectou!\n",inet_ntoa(endereco_cliente.sin_addr));
        lista_inserir(clientes, inet_ntoa(endereco_cliente.sin_addr),socket_clientes_atual);
        pthread_t gerenciaDados;
        if(pthread_create(&gerenciaDados, NULL, (void*)gerencia_dados, clientes->fim) != 0) erro("Erro ao criar thread de gerenciamento de clientes!");
    }
	
	return 0;
}