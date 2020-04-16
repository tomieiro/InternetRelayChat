#include "socket_utils.h"


int kill_descriptor[2];

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


void *write_sock(void *c){
    write(((conexao*)c)->self_socket, ((conexao*)c)->buffer, sizeof(((conexao*)c)->buffer));
    return NULL;
}

//Metodo que envia uma mensagem quando ja setada uma conexao
//args: (char*) Mensagem a ser enviada
//return: (int) Quantidade de caracteres enviados
int conexao::envia_mensagem(){
    pthread_t aux;
    pthread_create(&aux, NULL, write_sock, this);
    pthread_join(aux, NULL); 
    return 0;
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


//Construtor
conexao_servidor::conexao_servidor(){
        this->quantidade_clientes = 0;
}

//Funcao para matar corretamente o programa fechando descritor da socket
void die_corretly(int signal){
    close(kill_descriptor[0]);
    close(kill_descriptor[1]);
    exit(EXIT_SUCCESS);
}

//Metodo que cria uma conexao do servidor para terceiros
void conexao_servidor::cria_conexao(){
    //Criando Self_Socket com a socket()
	if((this->self_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!\n");

    //Tratando a interrupcao do processo pelo sinal SIGINT (Crtl + C)
    kill_descriptor[0] = this->self_socket;
    kill_descriptor[1] = this->socket_cliente_atual;
    signal(SIGINT,die_corretly);

    //Definindo parametros do endereco
    this->endereco_socket.sin_family = AF_INET; //Definindo familia do endereco de internet
    this->endereco_socket.sin_addr.s_addr = htonl(INADDR_ANY); //Subindo no endereco ip
    this->endereco_socket.sin_port = htons(PORTA); //Definindo porta
    
    //Reservando porta para o socket usando o bind()
    if(bind(this->self_socket, (struct sockaddr *)&(this->endereco_socket), sizeof(this->endereco_socket)) < 0) erro("Reserva de endereco falhou!\n");
   
    //Habilita o socket para receber conexoes
    if(listen(this->self_socket, this->quantidade_clientes) < 0) erro("Habilitar  de conexoes falhou!\n");
}


void *read_sock(void *c){
    socklen_t aux = sizeof(((conexao_servidor*)c)->endereco_socket);
    if((((conexao_servidor*)c)->socket_cliente_atual = accept(((conexao_servidor*)c)->self_socket, (struct sockaddr*)&(((conexao_servidor*)c)->endereco_socket), &aux))<0) ((conexao_servidor*)c)->erro("Falha ao aceitar conexoes!\n");
    read(((conexao_servidor*)c)->socket_cliente_atual,((conexao_servidor*)c)->buffer, sizeof(((conexao_servidor*)c)->buffer));
}


void conexao_servidor::recebe_envios(){
    //Aceita conexoes
    
    pthread_t aux;
    pthread_create(&aux, NULL, read_sock, this);
    pthread_join(aux, NULL); 
    
    //Rodando apenas quando o evento accept ocorre
    if(this->buffer[0] != 0){
        if(this->quantidade_clientes == MAX_CLIENTES) return; //Caso a quantidade de clientes estoure o maximo

        //Verificando se o endereco atual ja fez conexao alguma vez anteriormente
        static bool verificacao = true;
        for(int k=0; k<MAX_CLIENTES; k++){ //Rodando para todo o vetor de enderecos
            if(this->sockets_clientes[k] == socket_cliente_atual){
                verificacao = false;
            }
        }
        //Se verdadeiro que e sua primeira conexao, entra na lista
        if(verificacao){
            this->quantidade_clientes++;
            this->sockets_clientes[this->quantidade_clientes] = socket_cliente_atual;
        }
        verificacao = true;
    }
}


void conexao_servidor::envia_para_clientes(){
    
    for(int i=0; i<this->quantidade_clientes; i++){
        write(this->sockets_clientes[i], this->buffer, sizeof(this->buffer));  
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
	if((this->self_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!\n");
    
    //Definindo parametros do endereco
    this->endereco_socket.sin_family = AF_INET; //Definindo familia do endereco de internet
    this->endereco_socket.sin_addr.s_addr = inet_addr(this->ip);
    this->endereco_socket.sin_port = htons(PORTA); //Definindo porta
    
    //Realiza conexao com o servidor
    if(connect(this->self_socket, (struct sockaddr *)&(this->endereco_socket), sizeof(this->endereco_socket)) < 0) erro("Criacao de conexao falhou!\n");	
}


void conexao_cliente::recebe_mensagens(){
    read(this->self_socket, this->buffer, sizeof(this->buffer));
}


//Metodo que restarta a conexao com o servidor
void conexao_cliente::restart_conexao(){
    //Criando Socket com a socket()
	if((this->self_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!\n");
    //Realiza conexao com o servidor
    if(connect(this->self_socket, (struct sockaddr *)&(this->endereco_socket), sizeof(this->endereco_socket)) < 0) erro("Criacao de conexao falhou!\n");
}