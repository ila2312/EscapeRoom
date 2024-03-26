#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define DIM_BUFFER 1024
#define DIM_CMD 10
#define DIM_ACK 6


char * arrayOfComands [] = { "take", "look", "drop", "use", "objs", "end", "hint" };

int accesso_eseguito=0;
int partita_iniziata=0;
int ignore_arrayOfCommands=0;
int chiudi_tutto=0;
int tokenMassimi, tokenOttenuti;
int listener, socket_server, fdmax, ret, mio_num, num_ricevuto;
fd_set master, read_fds;
struct sockaddr_in getn_addr;

void recv_msg(int sd, char* buffer);
void send_msg(int sd, char* buffer);
void gestione_start();
void gestione_comandi_partita();
void print_help();
int is_insideArrayOfComands(char* command);
void gestione_messaggio_ricevuto(char* buffer);


int main(int argc, char* argv[]){
    struct sockaddr_in sv_addr;
    int sv_port, i;

	//se c'è un argomento è il numero di porta altrimenti 4242
	if(argc>1)
		sv_port=atoi(argv[1]);
	else
		sv_port=4242;

    //creazione e connessione del socket di comunicazione con il server
    socket_server=socket(AF_INET, SOCK_STREAM, 0);
    if(socket_server<0){
        perror("Error: ");
		exit(1);
    }
    memset(&sv_addr, 0, sizeof(sv_addr));
	sv_addr.sin_family=AF_INET;
	sv_addr.sin_port=htons(sv_port);
	inet_pton(AF_INET, "127.0.0.1", &sv_addr.sin_addr);
    ret=connect(socket_server, (struct sockaddr*)&sv_addr, sizeof(sv_addr));
    if(ret==-1) {
		perror("Error: ");
		exit(1);
	}

	//Inizializzazione dei set di file descriptor e della variabile fdmax per la funzione select
 	FD_ZERO(&master);
 	FD_ZERO(&read_fds);
 	FD_SET(0, &master); //standard input ha FD 0
    FD_SET(socket_server, &master);
	fdmax=socket_server;

	print_help();
    //ora posso gestire tutto il resto
    while(chiudi_tutto == 0){
        read_fds=master;

        ret = select(fdmax+1, &read_fds, NULL, NULL, NULL);
        if(ret < 0){
			perror("Errore: ");
			exit(1);
		}

        for(i = 0; i <= fdmax; i++) {
            if(FD_ISSET(i, &read_fds)){
                if(i == 0)
                    gestione_comandi_partita(); //stdin
            }
        }
    }

	printf("Partita terminata.\n");
    return 0;
}

void recv_msg(int sd, char* buffer){
    int len;
	uint16_t lmsg;
    ret=recv(sd, (void*)&lmsg, sizeof(uint16_t), 0);
	if(ret==-1){
		perror("Errore: ");
		exit(1);
    }
	len=ntohs(lmsg);
    ret=recv(sd, (void*)buffer, len, 0);
    if(ret==-1){
        perror("Errore: ");
        exit(1);
    }
}

void send_msg(int sd, char* buffer){
    int len;
    uint16_t lmsg;
	len=strlen(buffer)+1;
	lmsg=htons(len);

    ret=send(sd, (void*)&lmsg, sizeof(uint16_t), 0);
	if(ret==-1){
		perror("Errore: ");
		exit(1);
		}
    ret=send(sd, (void*)buffer, len, 0);
    if(ret==-1){
        perror("Errore: ");
        exit(1);
    }
}



void gestione_comandi_partita() {
    char buffer[DIM_BUFFER] = "\0";
    char cmd[DIM_CMD] = "\0";
    fgets(buffer, DIM_BUFFER, stdin);
    sscanf(buffer, "%s", cmd);

	if (is_insideArrayOfComands(cmd) != 0) {
		//wrong command or not implemented
		printf("comando non riconosciuto: %s \n", cmd);
		print_help();
		return;
	}
	printf("comando ricevuto: %s \n", cmd);
	send_msg(socket_server, buffer);
	recv_msg(socket_server, buffer);
	gestione_messaggio_ricevuto(buffer);
}

void print_help(){
	printf("\n\n");
	printf("***************Comandi disponibili***************\n");

	if (accesso_eseguito == 0)
	{
		printf("login <username> <password>\n");
		printf("signup <username> <password>\n");
	}
	else
	{
		if (partita_iniziata == 0)
		{
			printf("start <room> (Baita)\n");
		}
		else
		{
			printf("take <obj>\n");
			printf("look \n");
			printf("look <obj/location>\n");
			printf("drop <obj>\n");
			printf("use <obj> ...\n");
			printf("end\n");
			printf("hint\n");
			printf("hint <msg>\n");
		}
	}
	printf("exit\n");
	printf("help\n");
}

int is_insideArrayOfComands(char * stringa) {
	if (ignore_arrayOfCommands == 1) {
		return 0;
	}

	if (accesso_eseguito == 0) {
		if (strcmp(stringa, "login") == 0 || strcmp(stringa, "signup") == 0) {
			return 0;
		} else {
			return 1;
		}
	}

	if (partita_iniziata == 0) {
		if (strcmp(stringa, "start") == 0) {
			return 0;
		}
	}

	for (int i = 0; i < 8; i++) {
		if (strcmp(stringa, arrayOfComands[i]) == 0) {
			return 0;
		}
	}

	return 1;
}

void gestione_messaggio_ricevuto(char* buffer) {
	//il formato della risposta sarà:
	// <UtenteLoggato> | <PartitaIniziata> | <TokenMassimi> | <TokenOttenuti> | <msg....>
	//dividere il buffer in base al formato

	//NB atoi() converte char* in int e restituisce 0 se non ci sono numeri
	printf("da server messaggio ricevuto: %s \n", buffer);
	accesso_eseguito = atoi(strtok(buffer, "|"));
	partita_iniziata = atoi(strtok(NULL, "|"));
	tokenMassimi = atoi(strtok(NULL, "|"));
	tokenOttenuti = atoi(strtok(NULL, "|"));
	char* msg = strtok(NULL, "|");


}