#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "lista_clientes.h"
#include "lista_canal.h"
#include "macros.h"

//DEFININDO CONSTANTES

#define MAX_CLIENTES 3
#define SOCKET int
#define MAX_CANAIS 256