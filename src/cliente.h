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
#include "macros.h"

typedef int SOCKET;

//DEFININDO CONSTANTES DO PROGRAMA
#define REFRESH_TIME 0.05