#include "cliente.h"

SOCKET self_socket;
int QUIT = 0;

//Metodo que lanca um erro e termina o programa
//args: (const char*) Frase de erro
void erro(const char erro[100]){
    perror(erro);
    exit(EXIT_FAILURE);
}

//Funcao para matar corretamente o programa fechando descritor da socket
void die_corretly(int signal){
    close(self_socket);
    printf("\nSaindo...\n");
    exit(EXIT_SUCCESS);
}

void envia_mensagem(void *arg){
    char mensagem[TAM_MSG_MAX], *aux;
    aux = malloc(TAM_MAX_BUFFER*sizeof(char));
    int count;
    while(1){
		count = 0;
        scanf("%s", aux);
        printf("aux:%s\n",aux);
        if(!strcmp(aux, "/quit")) printf("QUIT?");
		while(1){
			if((strlen(aux) - count) > TAM_MSG_MAX - 1){
				strncpy(mensagem,&aux[count],TAM_MSG_MAX - 1);
				mensagem[TAM_MSG_MAX - 1] = '\0';
				count += TAM_MSG_MAX;
				send(self_socket, mensagem, TAM_MSG_MAX, 0);
			}else{
				strncpy(mensagem,&aux[count],TAM_MSG_MAX - 1);
				mensagem[TAM_MSG_MAX - 1] = '\0';
				send(self_socket, mensagem, TAM_MSG_MAX, 0);
				break;
			}
		}
    }
    free(aux);
}

void recebe_mensagem(void *arg){
	char mensagem[TAM_MSG_MAX];
	while(1){
		if(recv(self_socket, mensagem, TAM_MSG_MAX, 0) == 0) erro("Erro ao receber do servidor!\n");
		printf("%s\n", mensagem);
	}
}


int main(int argc, char *argv[]){
	signal(SIGINT,die_corretly);
	char ip[20]; //Endereco de IP do servidor
    printf("Digite o endereço do servidor (Digite 0.0.0.0 para local): ");
    scanf("%[^\n]", ip);
    
    //Criando Socket com a socket()
	if((self_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) erro("Criacao do Socket falhou!\n");
    
    struct sockaddr_in endereco_servidor;
    
    //Definindo parametros do endereco
	endereco_servidor.sin_family = PF_INET; //Definindo familia do endereco de internet
    endereco_servidor.sin_addr.s_addr = inet_addr(ip);
    endereco_servidor.sin_port = htons(PORTA); //Definindo porta
    
    //Realiza conexao com o servidor
    if(connect(self_socket, (struct sockaddr *)&(endereco_servidor), sizeof(endereco_servidor)) < 0) erro("Criacao de conexao falhou!\n");	
    
    pthread_t enviaMsg;
    if(pthread_create(&enviaMsg, NULL, (void*)envia_mensagem, NULL) != 0) erro("Erro ao criar thread de envio!");
    
    pthread_t recebeMsg;
    if(pthread_create(&recebeMsg, NULL, (void*)recebe_mensagem, NULL) != 0) erro("Erro ao criar thread de envio!");
    
    while(1){
		//Rodando ate encontrar o SIGINT(Ctrl + C)
        if(QUIT) break;
	}
	close(self_socket);
	return 0;
}
