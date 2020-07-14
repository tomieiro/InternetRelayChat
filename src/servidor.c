#include "servidor.h"

//DEFININDO SOCKETS DO SERVIDOR E DO CLIENTE ATUAL
SOCKET self_socket, socket_clientes_atual;
//DEFININDO LISTA DE CLIENTES
LISTA *clientes;
char canal[200];
char **subnets;
CANAL *tab;


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

//Função que verifica se o cliente digitou o comando ping e retorna pong
int ping(NO *atual, char *buffer){
    if(!strcmp(buffer, "/ping")){
        strcpy(buffer,"pong");
        send(atual->self_socket, buffer, TAM_MSG_MAX, 0);
        return 0;
    }
}

//Função Join
int join(NO *atual, char *buffer){
    if(!strncmp(buffer, "/join", 5)){
        //guarda em algum lugar o nome do canal e o cliente
        char nomeCanal[50];
        int i=6, j=0;
        while(buffer[i] != '\0'){
            nomeCanal[j++] = buffer[i++];
        }
        nomeCanal[j] = '\0';    
        //verifica se o canal já foi criado ou não
        //char *ip = busca_canal(tab, nomeCanal, MAX_CANAIS);        
        //enviar o ip do canal
        //strcpy(buffer, ip);
        //send(atual->self_socket, buffer, TAM_MSG_MAX, 0);
        return 0;
    }
}

void cria_tabelas(){
    printf("%li\n",htonl(INADDR_ANY));
    subnets = calculate_subnets(htonl(INADDR_ANY));
}

//Funcao que gerencia todos os clientes no servidor
//args:(NO*) No atual da lista do cliente que se conectou no momento
void gerencia_dados(NO *atual){
    signal(SIGINT,die_corretly);
    NO *aux;
    int recebidos;
    char buffer[TAM_MSG_MAX];
    recv(atual->self_socket, canal, TAM_MSG_MAX, 0); //recebe canal
    while(1){
        aux = clientes->inicio;
        recebidos = recv(atual->self_socket, buffer, TAM_MSG_MAX, 0);
        if(recebidos <= 0){ //Caso mensagem alguma seja recebido, entao
            for(int i=0; i<4; i++){ //Tentando mais 4 vezes receber a mensagem
                recebidos = recv(atual->self_socket, buffer, TAM_MSG_MAX, 0);
                if(recebidos > 0) goto erro_de_conexao;
            }
            erro_de_conexao:
            lista_remover_item(clientes, atual->ip);
            break;
        }
    
        if(!ping(atual, buffer)) aux = NULL;

        //verifica o comando join
        if(!join(atual, buffer)) aux = NULL;

        while(aux != NULL){
            if(aux->self_socket != atual->self_socket){
                send(aux->self_socket, buffer, TAM_MSG_MAX, 0);
            }
			aux = aux->proximo;
		}
        printf("%s\n",canal);
    }
}

//Main
int main(int argc, char *argv[]){

    //Criando tabela de canais
    tab = criar_tabela(MAX_CANAIS); 

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
    int count = 0;
    while(1){ //Rodando ate que CTRL + C seja usado
        //Aceitando conexoes dos clientes e abrindo uma thread para cada um
		socket_clientes_atual = accept(self_socket, (struct sockaddr*)&endereco_cliente, (socklen_t*)&aux);
        if(count <= MAX_CLIENTES){

        }else{
            printf("QUANTIDADE DE CLIENTES EXCEDIDA!\n");
        }
        printf("O IP: %s se conectou!\n",inet_ntoa(endereco_cliente.sin_addr));
        lista_inserir(clientes, inet_ntoa(endereco_cliente.sin_addr),socket_clientes_atual);
        pthread_t gerenciaDados;
        if(pthread_create(&gerenciaDados, NULL, (void*)gerencia_dados, clientes->fim) != 0) erro("Erro ao criar thread de gerenciamento de clientes!");
    }
	return 0;
}

