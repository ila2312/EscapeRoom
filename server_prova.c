#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()

void func(int connfd) 
{ 
    char buff[80]; 
    int n; 
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, 80); 
   
        // read the message from client and copy it in buffer 
        read(connfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
        bzero(buff, 80); 
        n = 0; 
        // copy server message in the buffer 
        while ((buff[n++] = getchar()) != '\n') 
            ; 
   
        // and send that buffer to client 
        write(connfd, buff, sizeof(buff)); 
   
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
} 

int main() {
    int ret, sd, connfd, len, sdmax;
    struct sockaddr_in addr, cli;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 

    memset(&addr, 0, sizeof(addr)); 
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    addr.sin_port = htons(4242);

    ret = bind(sd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 

    if ((listen(sd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 

    len = sizeof(cli);

    connfd = accept(sd, (struct sockaddr*)&cli, &len); 
    if (connfd < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server accept the client...\n"); 


    fd_set read_fds, master;
    FD_ZERO(&master); //svuotamento dell'insieme di descrittori master
 	FD_ZERO(&read_fds); //svuotamento dell'insieme di descrittori read_fds
 	FD_SET(0, &master);  //aggiunta del descrittore 0 all'insieme dei descrittori master
 	FD_SET(sd, &master); //aggiunta del descrittore sd all'insieme dei descrittori master

 	sdmax = sd;
	
    //permette il controllo di piu socket rilevando quelli pronti
    //select(numero del descrittore piu alto +1, set di descrittori - uno per la lettura e uno per la scrittura (null), 
    //       eccezioni (null), itervallo di timeout (null))
    ret = select(sdmax+1, &read_fds, NULL, NULL, NULL);



    func(connfd);

    close(sd);

}