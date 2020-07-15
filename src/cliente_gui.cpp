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
Fl_Input *canalchat=(Fl_Input *)0;
Fl_Return_Button *startChat=(Fl_Return_Button *)0;
Fl_Return_Button *bPing=(Fl_Return_Button *)0;
Fl_Input *nick=(Fl_Input *)0;
Fl_Return_Button *bNick=(Fl_Return_Button *)0;
Fl_Input *kick=(Fl_Input *)0;
Fl_Return_Button *bKick=(Fl_Return_Button *)0;
Fl_Input *mute=(Fl_Input *)0;
Fl_Return_Button *bMute=(Fl_Return_Button *)0;
Fl_Input *unmute=(Fl_Input *)0;
Fl_Return_Button *bUnmute=(Fl_Return_Button *)0;
Fl_Input *whois=(Fl_Input *)0;
Fl_Return_Button *bWhois =(Fl_Return_Button *)0;


//Definindo variaveis globais
SOCKET self_socket;

int QUIT = false;
int IP_EXISTS = false;
int ENVIAR = false;
int CONECTADO = false;
char *aux; //String auxiliar do buffer
char ip[20]; //Endereco de IP do servidor
char user[50];
char canal[200];
int canal_setado = false;


using namespace std;


//----------------INICIO DA GUI------------------------------------------------
// generated by Fast Light User Interface Designer (fluid) version 1.0304

//Funcao handle para receber e colocar as mensagens na tela
static void cb_receber(char *rec) {
    string concat;
    concat.append(rec); concat.append("\n");
    if(strcmp(rec,"")) buffer->append(concat.c_str()); //Coloca mensagem na tela
    mensagens->redraw();
    return;
}

//Funcao handle para o botao enviar
static void cb_bEnviar(Fl_Return_Button*, void*) {
    string concat;
    concat.append("\tYOU: "); concat.append(escrita->value()); concat.append("\n"); //Coloca sua msg na tela
    buffer->append(concat.c_str());
    strcpy(aux, user);
    strcat(aux, " : ");
    strcat(aux,escrita->value());
    escrita->value("");
    mensagens->redraw();
    ENVIAR = true;
    return;
}

//Funcao handle para o botao start
static void cb_bStartChatStart(Fl_Return_Button*, void*) {
    if(!strcmp(ipServ->value(),"") || !strcmp(ipServ->value(),"")) return;
    strcpy(ip, ipServ->value());
    strcpy(user, username->value());
    char aux_canal[250];
    strcpy(aux_canal,"");
    strcat(aux_canal,"/join#");
    strcat(aux_canal,canalchat->value());
    strcpy(canal, aux_canal);
    IP_EXISTS = true;
    ipServ->deactivate();
    ipServ->hide();
    canalchat->deactivate();
    canalchat->hide();
    username->deactivate();
    username->hide();
    bPing->show();
    nick->show();
    bNick->show();
    kick->show();
    bKick->show();
    mute->show();
    bMute->show();
    unmute->show();
    bUnmute->show();
    whois->show();
    bWhois->show();
    bEnviar->label("ENVIAR");
    bEnviar->callback((Fl_Callback*)cb_bEnviar);
    mensagens->show();
    escrita->show();
}

//Funcao que seta um canal
static void seta_canal(){
    strcpy(aux, canal);
    ENVIAR = true;
    return;
}

//Funcao handle para o botao de ping
static void cb_bPing(){
    buffer->append("Voce solicitou o comando ping ao servidor!\n");
    strcpy(aux, "/ping");
    mensagens->redraw();
    ENVIAR = true;
    return;
}

//Funcao handle para o botao de nick
static void cb_bNick(){
    buffer->append("Voce trocou o nick de um usuario!\n");
    char aux_local[256];
    strcat(aux_local, "/nickname#");
    strcat(aux_local, nick->value());
    strcpy(aux, aux_local);
    mensagens->redraw();
    ENVIAR = true;
    nick->value("");
    return;
}

//Funcao handle para o botao de kick
static void cb_bKick(){
    buffer->append("Voce kickou um usuario!\n");
    char aux_local[256];
    strcat(aux_local, "/kick#");
    strcat(aux_local, kick->value());
    strcpy(aux, aux_local);
    mensagens->redraw();
    ENVIAR = true;
    kick->value("");
    return;
}

//Funcao handle para o botao de mute
static void cb_bMute(){
    buffer->append("Voce mutou um usuario!\n");
    char aux_local[256];
    strcat(aux_local, "/mute#");
    strcat(aux_local, mute->value());
    strcpy(aux, aux_local);
    mensagens->redraw();
    ENVIAR = true;
    mute->value("");
    return;
}

//Funcao handle para o botao de unmute
static void cb_bUnmute(){
    buffer->append("Voce desmutou um usuario!\n");
    char aux_local[256];
    strcat(aux_local, "/unmute#");
    strcat(aux_local, unmute->value());
    strcpy(aux, aux_local);
    mensagens->redraw();
    ENVIAR = true;
    unmute->value("");
    return;
}

//Funcao handle para o botao de whois
static void cb_bWhois(){
    buffer->append("Voce solicitou whois em um usuario!\n");
    char aux_local[256];
    strcat(aux_local, "/whois#");
    strcat(aux_local, whois->value());
    strcpy(aux, aux_local);
    mensagens->redraw();
    ENVIAR = true;
    whois->value("");
    return;
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
      //username->hide(); //REMOVER ESSA LINHA QUANDO FOR IMPLEMENTAR MULTI USUARIO
    } // Fl_Input* username
    { // Cria input do canal
      canalchat = new Fl_Input(235, 255, 290, 30, "DIGITE O CANAL:");
      canalchat->labelfont(4);
      canalchat->textfont(4);
      //username->hide(); //REMOVER ESSA LINHA QUANDO FOR IMPLEMENTAR MULTI USUARIO
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
      escrita = new Fl_Input(10, 390, 535, 75);
      escrita->color((Fl_Color)23);
      escrita->labelfont(4);
      escrita->textfont(4);
      escrita->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      escrita->hide();
    } // Fl_Input* escrita
     // Criando botao de ping
      bPing = new Fl_Return_Button(10, 515, 140, 25, "PING");
      bPing->labelfont(4);
      bPing->callback((Fl_Callback*)cb_bPing);
      bPing->hide();
    }
    { // Criando botao de enviar
      bEnviar = new Fl_Return_Button(10, 485, 140, 25, "   START CHAT");
      bEnviar->labelfont(4);
      bEnviar->callback((Fl_Callback*)cb_bStartChatStart);
    } // Fl_Return_Button* bEnviar
    { // Cria buffer de escrita
      nick = new Fl_Input(169, 485, 60, 25);
      nick->color((Fl_Color)23);
      nick->labelfont(4);
      nick->textfont(4);
      nick->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      nick->hide();
    } // Fl_Input* escrita
    {
     // Criando botao de nick
      bNick = new Fl_Return_Button(170, 515, 60, 25, "NICK");
      bNick->labelfont(4);
      bNick->callback((Fl_Callback*)cb_bNick);
      bNick->hide();
    }
    { // Cria buffer de escrita
      kick = new Fl_Input(239, 485, 60, 25);
      kick->color((Fl_Color)23);
      kick->labelfont(4);
      kick->textfont(4);
      kick->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      kick->hide();
    } // Fl_Input* escrita
    { // Criando botao de enviar
      bKick = new Fl_Return_Button(240, 515, 60, 25, "KICK");
      bKick->labelfont(4);
      bKick->callback((Fl_Callback*)cb_bKick);
      bKick->hide();
    } // Fl_Return_Button* bEnviar
    { // Cria buffer de mute
      mute = new Fl_Input(309, 485, 60, 25);
      mute->color((Fl_Color)23);
      mute->labelfont(4);
      mute->textfont(4);
      mute->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      mute->hide();
    } // Fl_Input* escrita
    { // Criando botao de mute
      bMute = new Fl_Return_Button(310, 515, 60, 25, "MUTE");
      bMute->labelfont(4);
      bMute->callback((Fl_Callback*)cb_bMute);
      bMute->hide();
    } // Fl_Return_Button* bEnviar
    { // Cria buffer de unmute
      unmute = new Fl_Input(379, 485, 80, 25);
      unmute->color((Fl_Color)23);
      unmute->labelfont(4);
      unmute->textfont(4);
      unmute->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      unmute->hide();
    } // Fl_Input* escrita
    { // Criando botao de unmute
      bUnmute = new Fl_Return_Button(380, 515, 80, 25, "UNMUTE");
      bUnmute->labelfont(4);
      bUnmute->callback((Fl_Callback*)cb_bUnmute);
      bUnmute->hide();
    } // Fl_Return_Button* bEnviar
    { // Cria buffer de whois
      whois = new Fl_Input(469, 485, 80, 25);
      whois->color((Fl_Color)23);
      whois->labelfont(4);
      whois->textfont(4);
      whois->align(Fl_Align(FL_ALIGN_TOP_LEFT));
      whois->hide();
    } // Fl_Input* escritas
    { // Criando botao de whois
      bWhois = new Fl_Return_Button(470, 515, 80, 25, "WHOIS");
      bWhois->labelfont(4);
      bWhois->callback((Fl_Callback*)cb_bWhois);
      bWhois->hide();
    } // Fl_Return_Button* bEnviar
    {
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
    while(!QUIT){
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
    pthread_exit(NULL);
}

//Funcao que eh aberta na thread para receber mensagens
void *recebe_mensagem(void *args){
	char mensagem[TAM_MSG_MAX];
    int count;
	while(!QUIT){
		if(recv(self_socket, mensagem, TAM_MSG_MAX, 0) == 0) erro("Erro ao receber do servidor!\n");
        cb_receber(mensagem);
        fflush(stdout);
	}
    pthread_exit(NULL);
}

//Funcao que atualiza a janela a cada 0.05 segundos
void refresh_all(void*){
    janela->flush();
    Fl::repeat_timeout(REFRESH_TIME, refresh_all);
}

//Funcao que instancia a GUI
void *instanciaGui(void *args){
    make_window();
    Fl::add_timeout(REFRESH_TIME,refresh_all);
    Fl::run();
    QUIT = true;
    exit(EXIT_SUCCESS);
}


//Main
int main(int argc, char *argv[]){
    pthread_t gui;
    pthread_create(&gui,NULL,instanciaGui, NULL); //Abre uma socket para a GUI
	  signal(SIGTSTP,die_corretly);
    signal(SIGINT,die_corretly);    //Trata o sigint

    while(true){
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

            sleep(1);

            seta_canal();

            while(true){
                //Rodando ate encontrar o SIGINT(Ctrl + D)
                if(QUIT) break;
            }
        }
    }
	close(self_socket);
  free(canal);
  free(aux);
	return 0;
}
