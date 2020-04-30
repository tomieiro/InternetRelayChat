#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket_utils.h"
#include <pthread.h>

using namespace std;

conexao_servidor Conexao;

void *recebe_mensagem(void *args){
    char buffer[4096] = {0};
    while(true){
        int recebidos = Conexao.recebe_envios();
        if(buffer[0] != 0) Conexao.limpa_mensagem();
        
        if(Conexao.get_mensagem()[0] != 0){
            Conexao.envia_para_clientes();
            Conexao.limpa_mensagem();
        }

	}
}

int main(int argc, char *argv[]){
    printf("Servidor iniciado!\n");
    Conexao.cria_conexao();
    pthread_t recebeMensagemS;
    if(pthread_create(&recebeMensagemS, NULL,recebe_mensagem, NULL) != 0){
        exit(EXIT_FAILURE);
    }
    while(1){
        //Running until Ctrl + C
    }
    return EXIT_SUCCESS;
}