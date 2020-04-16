#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "socket_utils.h"

using namespace std;

int main(int argc, char *argv[]){
	conexao_servidor Conexao;
    Conexao.cria_conexao();
	//Esperando conexao com cliente
    while(true){
        Conexao.recebe_envios();
        if(Conexao.get_mensagem()[0] != 0){
            //Conexao.envia_para_clientes();
            Conexao.limpa_mensagem();
        }
	}
}