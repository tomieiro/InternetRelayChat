#include <string>
#include "cliente.h"
#include "gui.h"

//Instanciando elementos da GUI
Fl_Double_Window *janela=(Fl_Double_Window *)0;
Fl_Text_Display *mensagens=(Fl_Text_Display *)0;
Fl_Input *escrita=(Fl_Input *)0;
Fl_Text_Buffer *buffer = (Fl_Text_Buffer*)0;
Fl_Return_Button *bEnviar=(Fl_Return_Button *)0;
Fl_Input *ipServ=(Fl_Input *)0;
Fl_Input *username=(Fl_Input *)0;
Fl_Return_Button *startChat=(Fl_Return_Button *)0;

//Definindo variaveis globais
SOCKET self_socket;

int QUIT = false;
int IP_EXISTS = false;
int ENVIAR = false;
char *aux; //String auxiliar do buffer
char ip[20]; //Endereco de IP do servidor


using namespace std;


//----------------INICIO DA GUI------------------------------------------------
// generated by Fast Light User Interface Designer (fluid) version 1.0304

//Funcao handle para receber e colocar as mensagens na tela
static void cb_receber(char *rec) {
    string concat;
    concat.append("USER_2: "); concat.append(rec); concat.append("\n"); 
    if(strcmp(rec,"")) buffer->append(concat.c_str()); //Coloca mensagem na tela
    mensagens->redraw();
    return;
}

//Funcao handle para o botao enviar
static void cb_bEnviar(Fl_Return_Button*, void*) {
    string concat;
    concat.append("\tYOU: "); concat.append(escrita->value()); concat.append("\n"); //Coloca sua msg na tela
    buffer->append(concat.c_str());
    strcpy(aux,escrita->value());
    escrita->value("");
    mensagens->redraw();
    ENVIAR = true;
    return;
}

//Funcao handle para o botao start
static void cb_bStartChatStart(Fl_Return_Button*, void*) {
    if(!strcmp(ipServ->value(),"") || !strcmp(ipServ->value(),"")) return;
    strcpy(ip, ipServ->value());
    IP_EXISTS = true;
    ipServ->deactivate();
    ipServ->hide();
    username->deactivate();
    username->hide();
    bEnviar->label("ENVIAR");
    bEnviar->callback((Fl_Callback*)cb_bEnviar);
    mensagens->show();
    escrita->show();
}

/**
 Funcao que cria uma janela
*/
Fl_Double_Window* make_window() {
  { // Criando janela
    janela = new Fl_Double_Window(555, 555, "InternetRelayChat");
    janela->color((Fl_Color)14);
    janela->labelfont(4);
    janela->align(Fl_Align(FL_ALIGN_CENTER));
    { // Cria input do ip
      ipServ = new Fl_Input(235, 165, 290, 30, "DIGITE O IP DO SERVIDOR:");
      ipServ->labelfont(4);
      ipServ->textfont(4);
    } // Fl_Input* ipServ
    { // Cria input do ip
      username = new Fl_Input(235, 210, 290, 30, "DIGITE SEU USERNAME:");
      username->labelfont(4);
      username->textfont(4);
      username->hide(); //REMOVER ESSA LINHA QUANDO FOR IMPLEMENTAR MULTI USUARIO
    } // Fl_Input* username
    {
      buffer = new Fl_Text_Buffer();
    }
    { // Cria buffer de mensagens
      mensagens = new Fl_Text_Display(10, 10, 535, 375);
      mensagens->buffer(buffer);
      mensagens->line_end(10,true);
      mensagens->labelfont(4);
      mensagens->textfont(4);
      mensagens->hide();
    } // Fl_Text_Display* mensagens
    { // Cria buffer de escrita
      escrita = new Fl_Input(10, 390, 535, 115);
      escrita->color((Fl_Color)23);
      escrita->labelfont(4);
      escrita->textfont(4);
      escrita->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      escrita->hide();
    } // Fl_Input* escrita
    { // Criando botao de enviar
      bEnviar = new Fl_Return_Button(10, 515, 535, 25, "   START CHAT");
      bEnviar->labelfont(4);
      bEnviar->callback((Fl_Callback*)cb_bStartChatStart);
    } // Fl_Return_Button* bEnviar
    janela->end();
    janela->show();
  } // Fl_Double_Window* janela
  return janela;
}

//-------------------FIM DA GUI------------------------------------------------


//Metodo que lanca um erro e termina o programa
//args: (const char*) Frase de erro
void erro(const char erro[100]){
    perror(erro);
    exit(EXIT_FAILURE);
}

//Funcao para matar corretamente o programa fechando descritor da socket
void die_corretly(int signal){
    close(self_socket);
    free(aux);
    printf("\nSaindo...\n");
    exit(EXIT_SUCCESS);
}

//Funcao que eh aberta na thread para enviar mensagens
void *envia_mensagem(void *args){
    char mensagem[TAM_MSG_MAX];
    string str;
    aux =(char*) malloc(TAM_MAX_BUFFER*sizeof(char));
    int count;
    while(1){
		count = 0;
        if(ENVIAR){
            if(!strcmp(aux, "/quit")) QUIT = true;
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
                    ENVIAR = false;
                    break;
                }
            }

        }
    }
}

//Funcao que eh aberta na thread para receber mensagens
void *recebe_mensagem(void *args){
	char mensagem[TAM_MSG_MAX];
    int count;
	while(1){
		if(recv(self_socket, mensagem, TAM_MSG_MAX, 0) == 0) erro("Erro ao receber do servidor!\n");
        cb_receber(mensagem);
        fflush(stdout);
	}
}

//Funcao que instancia a GUI
void *instanciaGui(void *args){
    make_window();
    Fl::run();
    close(self_socket);
    free(aux);
    exit(EXIT_SUCCESS);
}


//Main
int main(int argc, char *argv[]){
    pthread_t gui;
    pthread_create(&gui,NULL,instanciaGui, NULL); //Abre uma socket para a GUI
	signal(SIGINT,die_corretly); //Trata o sigint

    while(1){
        if(IP_EXISTS and !QUIT){ //Se o usuario digitar o ip
            //Criando Socket com a socket()
            if((self_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) erro("Criacao do Socket falhou!\n");
            
            struct sockaddr_in endereco_servidor;
            
            //Definindo parametros do endereco
            endereco_servidor.sin_family = PF_INET; //Definindo familia do endereco de internet
            endereco_servidor.sin_addr.s_addr = inet_addr(ip);
            endereco_servidor.sin_port = htons(PORTA); //Definindo porta
            
            //Realiza conexao com o servidor
            if(connect(self_socket, (struct sockaddr *)&(endereco_servidor), sizeof(endereco_servidor)) < 0) erro("Criacao de conexao falhou!\n");	
            
            //Abrindo uma thread para enviar mensagens
            pthread_t enviaMsg;
            if(pthread_create(&enviaMsg, NULL, envia_mensagem, NULL) != 0) erro("Erro ao criar thread de envio!");
            
            //Abrindo uma thread para receber mensagens
            pthread_t recebeMsg;
            if(pthread_create(&recebeMsg, NULL, recebe_mensagem, NULL) != 0) erro("Erro ao criar thread de envio!");
            
            while(1){
                //Rodando ate encontrar o SIGINT(Ctrl + C)
                if(QUIT) break;
            }
        }
    }
	close(self_socket);
    free(aux);
	return 0;
}