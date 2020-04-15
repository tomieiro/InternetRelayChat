#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

//Importando bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
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
    char buffer[4096];
    conexao();
    void erro(const char*);
    void cria_conexao();
    void set_mensagem(char*);
    int envia_mensagem();
    void limpa_mensagem();
    void finaliza_conexao();
    char *get_mensagem();
    
};

//Classe servidor
class conexao_servidor : public conexao {
    struct sockaddr_in endereco_sockets_clientes[MAX_CLIENTES];
    socklen_t tam_endereco_cliente[MAX_CLIENTES];
    int socket_cliente_atual;
    int quantidade_clientes;
    public:
    conexao_servidor();
    void cria_conexao();
    void recebe_envios();
};

//Classe cliente
class conexao_cliente : public conexao {
    char *ip;
    public:
    void cria_conexao(char*);
    void restart_conexao();
};

#endif