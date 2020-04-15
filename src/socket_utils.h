#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

//Importando bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


//DEFININDO CONSTANTES
#define PORTA 31010
#define MAX_CLIENTES 3


//Classe base
class conexao{
    public:
    int self_socket;
    struct sockaddr_in endereco_socket;
    void erro(const char*);
    void cria_conexao();
    int envia_mensagem(char*);
    void finaliza_conexao();
};

//Classe servidor
class conexao_servidor : public conexao {
    struct sockaddr_in endereco_sockets_clientes[MAX_CLIENTES];
    socklen_t tam_endereco_cliente[MAX_CLIENTES];
    int socket_cliente_atual;
    int quantidade_clientes;
    public:
    conexao_servidor(){
        for(int i=0; i<MAX_CLIENTES; i++){
            this->tam_endereco_cliente[i] = 0;
        }
    }
    void cria_conexao();
    void recebe_envios(char*);
};

//Classe cliente
class conexao_cliente : public conexao {
    char *ip;
    public:
    void cria_conexao(char*);
    void restart_conexao();
};

#endif