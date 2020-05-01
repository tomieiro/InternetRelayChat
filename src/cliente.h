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

#define PORTA 31010
#define TAM_MSG_MAX 4096
#define TAM_MAX_BUFFER (TAM_MSG_MAX*100)
typedef int SOCKET;
