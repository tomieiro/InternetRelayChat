
typedef struct canal{
    char nome[200];
    char ip[20];
}CANAL;


CANAL* criar_tabela(int tam);
void inserir_canal(CANAL* tab,int ind,char ip[20], char* nome);
int hash(int value,int tam);
char* busca_hash(CANAL* tab, char* nome, int tam);


