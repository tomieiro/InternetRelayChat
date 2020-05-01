#include "socket_utils.h"

int kill_descriptor[2];
LISTA *L;

//METODOS DA CLASSE MAE CONEXAO

//Construtor
conexao::conexao(){
    bzero(this->buffer, 4096);
}

//Metodo que lanca um erro e termina o programa
//args: (const char*) Frase de erro
void conexao::erro(const char erro[100]){
        perror(erro);
        exit(EXIT_FAILURE);
}

void conexao::set_mensagem(char b[4096]){
    strcpy(this->buffer, b);
}

char *conexao::get_mensagem(){
    return this->buffer;
}

//Metodo que envia uma mensagem quando ja setada uma conexao
//args: (char*) Mensagem a ser enviada
//return: (int) Quantidade de caracteres enviados
int conexao::envia_mensagem(){
    return send(this->self_socket, this->buffer, 4096, 0);
}

//Metodo que limpa o buffer
void conexao::limpa_mensagem(){
    for(int i=0; i<4096; i++){
        this->buffer[i] = 0;
    }
}

//Metodo que finaliza uma conexao
void conexao::finaliza_conexao(){
    close(this->self_socket);
}

//METODOS DA CLASSE FILHA SERVIDOR

conexao_servidor::conexao_servidor(){
    this->sockets_clientes = lista_criar();
    L = this->sockets_clientes;
}

//Funcao para matar corretamente o programa fechando descritor da socket
void die_corretly(int signal){
    close(kill_descriptor[0]);
    close(kill_descriptor[1]);
    lista_apagar(L);
    printf("\nFechando servidor...\n");
    exit(EXIT_SUCCESS);
}

//Metodo que cria uma conexao do servidor para terceiros
void conexao_servidor::cria_conexao(){
    //Criando Self_Socket com a socket()
	if((this->self_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) erro("Criacao do Socket falhou!\n");

    //Tratando a interrupcao do processo pelo sinal SIGINT (Crtl + C)
    kill_descriptor[0] = this->self_socket;
    kill_descriptor[1] = this->socket_cliente_atual;
    signal(SIGINT,die_corretly);

    //Definindo parametros do endereco
    this->endereco_socket.sin_family = PF_INET; //Definindo familia do endereco de internet
    this->endereco_socket.sin_addr.s_addr = htonl(INADDR_ANY); //Subindo no endereco ip
    this->endereco_socket.sin_port = htons(PORTA); //Definindo porta
    
    //Reservando porta para o socket usando o bind()
    if(bind(this->self_socket, (struct sockaddr *)&(this->endereco_socket), sizeof(this->endereco_socket)) < 0) erro("Reserva de endereco falhou!\n");
   
    //Habilita o socket para receber conexoes
    if(listen(this->self_socket, MAX_CLIENTES) < 0) erro("Habilitar  de conexoes falhou!\n");
}

int conexao_servidor::recebe_envios(){

    //Endereco e seu tamanho declarados
    struct sockaddr_in aux_addr;
    socklen_t aux = sizeof(aux_addr);
    
    //Aceita conexoes
    if((this->socket_cliente_atual = accept(this->self_socket, (struct sockaddr*)&(aux_addr), &aux))<0) erro("Falha ao aceitar conexoes!\n");
    int recebidos = recv(this->socket_cliente_atual, this->buffer, 4096, 0);

    //Rodando apenas quando o evento accept ocorre
    if(this->buffer[0] != 0){
        if(lista_tamanho(this->sockets_clientes) == MAX_CLIENTES) return 0; //Caso a quantidade de clientes estoure o maximo

        //Verificando se o endereco atual ja fez conexao alguma vez anteriormente
        static bool verificacao = false;
        if(lista_buscar_item(this->sockets_clientes,this->socket_cliente_atual) == -404){
            verificacao = true;
        }

        //Se verdadeiro que e sua primeira conexao, entra na lista
        if(verificacao){
            printf("SIZE: %d\n",lista_tamanho(this->sockets_clientes));
            lista_inserir_fim(this->sockets_clientes,socket_cliente_atual);
            verificacao = false;
        }
    }
    return recebidos;
}


void conexao_servidor::envia_para_clientes(){
    int elemento;
    char ip[20];
    if(lista_tamanho(this->sockets_clientes) == 0) return;
    send(this->socket_cliente_atual, this->buffer, 4096, 0); return;
    for(int i=0; i<lista_tamanho(this->sockets_clientes); i++){
        lista_at(this->sockets_clientes, i, &elemento, ip);
        send(elemento, this->buffer, 4096, 0);
    }
}



//METODOS DA CLASSE FILHA CLIENTE

//Funcao para matar corretamente o programa fechando descritor da socket
void die_corretly_cliente(int signal){
    close(kill_descriptor[0]);
    exit(EXIT_SUCCESS);
}

//Metodo que cria uma conexao de um cliente com o servidor
//args: (char*) Endereco de ip do servidor
void conexao_cliente::cria_conexao(char ip[20]){
    //Setando ip
    this->ip = ip;

    //Tratando a interrupcao do processo pelo sinal SIGINT (Crtl + C)
    kill_descriptor[0] = this->self_socket;
    signal(SIGINT,die_corretly_cliente);

    //Criando Socket com a socket()
	if((this->self_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) erro("Criacao do Socket falhou!\n");
    
    //Definindo parametros do endereco
    this->endereco_socket.sin_family = PF_INET; //Definindo familia do endereco de internet
    this->endereco_socket.sin_addr.s_addr = inet_addr(this->ip);
    this->endereco_socket.sin_port = htons(PORTA); //Definindo porta
    
    //Realiza conexao com o servidor
    if(connect(this->self_socket, (struct sockaddr *)&(this->endereco_socket), sizeof(this->endereco_socket)) < 0) erro("Criacao de conexao falhou!\n");	
}


//Metodo que envia uma mensagem quando ja setada uma conexao
//args: (char*) Mensagem a ser enviada
//return: (int) Quantidade de caracteres enviados
int conexao_cliente::envia_mensagem(){
    return send(this->self_socket, this->buffer, 4096, 0);
}


int conexao_cliente::recebe_mensagens(){
    return recv(this->self_socket, this->buffer, 4096, 0);
}


//Metodo que restarta a conexao com o servidor
void conexao_cliente::restart_conexao(){
    //Criando Socket com a socket()
	if((this->self_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) erro("Criacao do Socket falhou!\n");
    //Realiza conexao com o servidor
    if(connect(this->self_socket, (struct sockaddr *)&(this->endereco_socket), sizeof(this->endereco_socket)) < 0) erro("Criacao de conexao falhou!\n");
}