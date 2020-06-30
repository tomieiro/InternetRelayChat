
typedef struct canal{
    char nome[200];
    char ip[20];
}CANAL;


CANAL* criar_tabela(int tam);
void inserir_canal(CANAL* tab,int ind,char ip[20], char* nome);
int hash(char* nome,int tam);
char* verifica_canal(CANAL* tab, char* nome, int tam);
