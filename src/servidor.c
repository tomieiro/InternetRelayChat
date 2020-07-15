#include "servidor.h"

//DEFININDO SOCKETS DO SERVIDOR E DO CLIENTE ATUAL
SOCKET self_socket, socket_clientes_atual;
//DEFININDO LISTA DE CLIENTES
LISTA_CANAL *canais;
char canal[200];


//Metodo que lanca um erro e termina o programa
//args: (const char*) Frase de erro
void erro(const char erro[100]){
    perror(erro);
    exit(EXIT_FAILURE);
}

//Funcao para matar corretamente o programa fechando descritor da socket
void die_corretly(int signal){
    CANAL *aux_canais = canais->inicio;
    while(aux_canais != NULL){
        NO *aux = aux_canais->clientes->inicio;
        while(aux != NULL){
            close(aux->self_socket);
            aux = aux->proximo;
        }
        close(self_socket);
        lista_apagar(aux_canais->clientes);
        aux_canais = aux_canais->proximo;
    }
    lista_canais_apagar(canais);
    printf("\nFechando servidor...\n");
    exit(EXIT_SUCCESS);
}

//Função que verifica se o cliente digitou o comando ping e retorna pong
int ping(NO *atual, char *buffer){
    if(!strcmp(buffer, "/ping")){
        strcpy(buffer,"pong");
        send(atual->self_socket, buffer, TAM_MSG_MAX, 0);
        return 0;
    }
}

//Função que kicka um cliente do server
int kick(NO *atual, char *buffer){
    if(!strncmp(buffer, "/kick", 5)){
        
        return 0;
    }
}

//Função que troca o nick de um cliente
int nick(NO *atual, char *buffer){
    if(!strncmp(buffer, "/nick", 5)){
        
        return 0;
    }
}

//Função que muta um cliente
int mute(NO *atual, char *buffer){
    if(!strncmp(buffer, "/mute", 5)){
        
        return 0;
    }
}

//Função que desmuta um cliente
int unmute(NO *atual, char *buffer){
    if(!strncmp(buffer, "/unmute", 7)){
        
        return 0;
    }
}

//Função que 
int whois(NO *atual, char *buffer){
    if(!strncmp(buffer, "/whois", 6)){
        //Retorna ip do cara que tem esse nome
        return 0;
    }
}


//Funcao que gerencia todos os clientes no servidor
//args:(NO*) No atual da lista do cliente que se conectou no momento
void *gerencia_dados(void *c_atual){
    CANAL *canal_atual = (CANAL*)c_atual;
    NO *cliente_atual = canal_atual->clientes->fim;
    signal(SIGINT,die_corretly);
    NO *aux;
    int recebidos;
    char buffer[TAM_MSG_MAX];
    while(1){
        aux = canal_atual->clientes->inicio;
        recebidos = recv(cliente_atual->self_socket, buffer, TAM_MSG_MAX, 0);
        if(recebidos <= 0){ //Caso mensagem alguma seja recebido, entao
            for(int i=0; i<4; i++){ //Tentando mais 4 vezes receber a mensagem
                recebidos = recv(cliente_atual->self_socket, buffer, TAM_MSG_MAX, 0);
                if(recebidos > 0) goto erro_de_conexao;
            }
            erro_de_conexao:
            lista_remover_item(canal_atual->clientes, cliente_atual->ip);
            break;
        }
    
        if(!ping(cliente_atual, buffer)) aux = NULL;

        while(aux != NULL){
            if(aux->self_socket != cliente_atual->self_socket){
                send(aux->self_socket, buffer, TAM_MSG_MAX, 0);
            }
			aux = aux->proximo;
		}
        printf("CANAL: %s\n",canal);
    }
}

//Main
int main(int argc, char *argv[]){

    //Criando lista
    CANAL *canal_atual;
    canais = lista_canais_criar();
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
    int count = 0;
    while(1){ //Rodando ate que CTRL + C seja usado
        //Aceitando conexoes dos clientes e abrindo uma thread para cada um
		socket_clientes_atual = accept(self_socket, (struct sockaddr*)&endereco_cliente, (socklen_t*)&aux);
        if(count <= MAX_CLIENTES){

        }else{
            printf("QUANTIDADE DE CLIENTES EXCEDIDA!\n");
        }
        recv(socket_clientes_atual, canal, 200, 0); //recebe canal
        CANAL *aux_canal_atual;
        aux_canal_atual = lista_canais_buscar_item(canais,canal);
        if(aux_canal_atual == NULL){ //Se o canal nao existir, cria-se um novo
            canal_atual->clientes = lista_criar();
            strcpy(canal_atual->nome_canal,canal);
            canal_atual->proximo = NULL;
            lista_canais_inserir(canais,canal_atual);
            lista_inserir(canal_atual->clientes,inet_ntoa(endereco_cliente.sin_addr),socket_clientes_atual);
        }
        else{ //Se ja existir, acrescenta o usuario atual a sua lista de usuarios
            lista_inserir(aux_canal_atual->clientes,inet_ntoa(endereco_cliente.sin_addr),socket_clientes_atual);
        }
        printf("O IP: %s se conectou!\n",inet_ntoa(endereco_cliente.sin_addr));
        pthread_t gerenciaDados; // Abre uma thread para o cara recem conectado
        if(pthread_create(&gerenciaDados, NULL, gerencia_dados, canal_atual) != 0) erro("Erro ao criar thread de gerenciamento de clientes!");
    }
	return 0;
}

