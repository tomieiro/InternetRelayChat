#include "socket_utils.h"


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
    printf("STRING: %s\n",b);
    strcpy(this->buffer, b);
}

char *conexao::get_mensagem(){
    return this->buffer;
}

//Metodo que envia uma mensagem quando ja setada uma conexao
//args: (char*) Mensagem a ser enviada
//return: (int) Quantidade de caracteres enviados
int conexao::envia_mensagem(){
    return send(this->self_socket, this->buffer, strlen(this->buffer), 0);
}

//Metodo que limpa o buffer
void conexao::limpa_mensagem(){
    bzero(this->buffer, 4096);
}

//Metodo que finaliza uma conexao
void conexao::finaliza_conexao(){
    close(this->self_socket);
}

//METODOS DA CLASSE FILHA SERVIDOR


//Construtor
conexao_servidor::conexao_servidor(){
        for(int i=0; i<MAX_CLIENTES; i++){
            this->tam_endereco_cliente[i] = 0;
        }
    }

//Metodo que cria uma conexao do servidor para terceiros
void conexao_servidor::cria_conexao(){
    //Criando Self_Socket com a socket()
	if((this->self_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!\n");
    
    //Definindo parametros do endereco
    this->endereco_socket.sin_family = AF_INET; //Definindo familia do endereco de internet
    this->endereco_socket.sin_addr.s_addr = htonl(INADDR_ANY); //Subindo no endereco ip
    this->endereco_socket.sin_port = htons(PORTA); //Definindo porta
    
    //Reservando porta para o socket usando o bind()
    if(bind(this->self_socket, (struct sockaddr *)&(this->endereco_socket), sizeof(this->endereco_socket)) < 0) erro("Reserva de endereco falhou!\n");
   
    //Habilita o socket para receber conexoes
    if(listen(this->self_socket, this->quantidade_clientes) < 0) erro("Habilitar  de conexoes falhou!\n");
}

void conexao_servidor::recebe_envios(){
    //Endereco e seu tamanho declarados
    //static struct sockaddr_in *aux_addr;
    //static socklen_t aux = sizeof(this->endereco_sockets_clientes);
    //Aceita conexoes
    if((this->socket_cliente_atual = accept(this->self_socket, (struct sockaddr*)&(this->endereco_sockets_clientes[0])/*aux_addr*/, &this->tam_endereco_cliente[0]/*aux*/))<0) erro("Falha ao aceitar conexoes!\n");
    recv(this->socket_cliente_atual, this->buffer, 4096, 0);
    /*
    if(aux_addr->sa_data != 0){
        for(int k=0; k<MAX_CLIENTES; k++){
            if(this->endereco_sockets_clientes[k].sin_addr.s_addr == aux_t) 
        }
        if(this->quantidade_clientes < MAX_CLIENTES){
                
            }
        aux_addr->sa_data = 0;
    }
    */
}


//METODOS DA CLASSE FILHA CLIENTE


//Metodo que cria uma conexao de um cliente com o servidor
//args: (char*) Endereco de ip do servidor
void conexao_cliente::cria_conexao(char ip[20]){
    //Setando ip
    this->ip = ip;

    //Criando Socket com a socket()
	if((this->self_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!\n");
    
    //Definindo parametros do endereco
    this->endereco_socket.sin_family = AF_INET; //Definindo familia do endereco de internet
    this->endereco_socket.sin_addr.s_addr = inet_addr(this->ip);
    this->endereco_socket.sin_port = htons(PORTA); //Definindo porta
    
    //Realiza conexao com o servidor
    if(connect(this->self_socket, (struct sockaddr *)&(this->endereco_socket), sizeof(this->endereco_socket)) < 0) erro("Criacao de conexao falhou!\n");	
}


//Metodo que restarta a conexao com o servidor
void conexao_cliente::restart_conexao(){
    //Criando Socket com a socket()
	if((this->self_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!\n");
    //Realiza conexao com o servidor
    if(connect(this->self_socket, (struct sockaddr *)&(this->endereco_socket), sizeof(this->endereco_socket)) < 0) erro("Criacao de conexao falhou!\n");
}