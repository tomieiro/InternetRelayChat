
typedef struct canal{
    char admin_ip[20]; // Quem comanda o Canal
    char nome[200];
    char ip[20]; // Onde o canal está hosteado
}CANAL;


CANAL* criar_tabela(int tam);
void inserir_canal(CANAL* tab, int tam, char* ip, char* admin_ip, char* nome);
int hash_ind(char* nome,int tam);
char* verifica_canal(CANAL* tab, char* nome, int tam);
char** calculate_subnet(char* ip);
char* busca_canal(CANAL* tab, char* nome, char* user_ip,int tam);
