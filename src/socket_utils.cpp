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



//Metodo que envia uma mensagem quando ja setada uma conexao
//args: (char*) Mensagem a ser enviada
//return: (int) Quantidade de caracteres enviados
int conexao::envia_mensagem(){
    return send(this->self_socket, this->buffer, strlen(this->buffer), 0);
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
    this->sockets_clientes.get_allocator().allocate(MAX_CLIENTES);
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
    if(listen(this->self_socket, MAX_CLIENTES) < 0) erro("Habilitar  de conexoes falhou!\n");
}

void conexao_servidor::recebe_envios(){

    //Endereco e seu tamanho declarados
    struct sockaddr_in aux_addr;
    socklen_t aux = sizeof(aux_addr);
    
    //Aceita conexoes
    if((this->socket_cliente_atual = accept(this->self_socket, (struct sockaddr*)&(aux_addr), &aux))<0) erro("Falha ao aceitar conexoes!\n");
    recv(this->socket_cliente_atual, this->buffer, 4096, 0);
    
    //Rodando apenas quando o evento accept ocorre
    if(this->buffer[0] != 0){
        if(this->sockets_clientes.size() == MAX_CLIENTES) return; //Caso a quantidade de clientes estoure o maximo

        //Verificando se o endereco atual ja fez conexao alguma vez anteriormente
        static bool verificacao = true;
        for(int k=1; k<=this->sockets_clientes.size(); k++){ //Rodando para todo o vetor de enderecos
            if(this->sockets_clientes.at(k) == socket_cliente_atual){
                verificacao = false;
    if(aux != 0){

        //Aceita conexoes
        socklen_t aux = sizeof(this->endereco_socket);
        if((this->socket_cliente_atual = accept(this->self_socket, (struct sockaddr*)&(this->endereco_socket), &aux))<0) erro("Falha ao aceitar conexoes!\n");
        
        recv(socket_cliente_atual, this->buffer, sizeof(this->buffer), 0);

        //Rodando apenas quando o evento accept ocorre
        if(this->buffer[0] != 0){

            if(this->quantidade_clientes == MAX_CLIENTES) return; //Caso a quantidade de clientes estoure o maximo

            //Verificando se o endereco atual ja fez conexao alguma vez anteriormente
            static bool verificacao = true;
            for(int k=0; k<MAX_CLIENTES; k++){ //Rodando para todo o vetor de enderecos


                if(this->sockets_clientes[k] == socket_cliente_atual){

                    verificacao = false;
                }
        //Se verdadeiro que e sua primeira conexao, entra na lista
        if(verificacao){
            this->sockets_clientes.push_back(socket_cliente_atual);
        }
        verificacao = true;

    }
}


//Metodo que repassa mensagem para os clientes
/*
void conexao_servidor::repassa_mensagens(){
    
    int socket_cliente; 
    socklen_t size_addr;    

     

    for(int i = 0; i < this->quantidade_clientes; i++){
        //Cria um novo socket para o cliente
        if((socket_cliente = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!\n");

        std::cout << endereco_sockets_clientes[i].sin_port << std::endl;

        size_addr = sizeof(this->endereco_sockets_clientes[i]);
        //Conecta o endereço do cliente ao socket
        if(connect(socket_cliente, (struct sockaddr *) &(this->endereco_sockets_clientes[i]), size_addr) < 0) erro("Não conseguiu conectar\n");
        
        //Envia mensagem
        if(send(socket_cliente, this->buffer, strlen(this->buffer), 0) < 0) erro("Não conseguiu enviar mensagem");

        close(socket_cliente);

    }

}
*/

/*
void conexao_servidor::envia_para_clientes(){
    if(this->sockets_clientes.size() == 0) return;
    for(int i=1; i<=this->sockets_clientes.size(); i++){
        send(this->sockets_clientes.at(i), this->buffer, sizeof(this->buffer), 0);  
    }
}
*/

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


//Metodo que envia uma mensagem quando ja setada uma conexao
//args: (char*) Mensagem a ser enviada
//return: (int) Quantidade de caracteres enviados
int conexao_cliente::envia_mensagem(){
    return send(this->self_socket, this->buffer, sizeof((this->buffer)), 0);
}


int conexao_cliente::recebe_mensagens(){
    return recv(this->self_socket, this->buffer, sizeof(this->buffer), 0);
}


//Metodo que restarta a conexao com o servidor
void conexao_cliente::restart_conexao(){
    //Criando Socket com a socket()
	if((this->self_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) erro("Criacao do Socket falhou!\n");
    //Realiza conexao com o servidor
    if(connect(this->self_socket, (struct sockaddr *)&(this->endereco_socket), sizeof(this->endereco_socket)) < 0) erro("Criacao de conexao falhou!\n");

}

