#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()

void func(int sockfd)
{
    char buff[80];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main() {
    int ret, sd, connfd, len, socket_server, sdmax;
    struct sockaddr_in addr, sv_addr;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(4242);

    len = sizeof(addr);

    if (connect(sd, (struct sockaddr*)&addr, len)!= 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    
    socket_server=socket(AF_INET, SOCK_STREAM, 0);
    memset(&sv_addr, 0, sizeof(sv_addr));

    fd_set read_fds, master;
    FD_ZERO(&master);
 	FD_ZERO(&read_fds);
 	FD_SET(0, &master);
 	FD_SET(sd, &master);
    FD_SET(socket_server, &master);
    if(sd>socket_server)
 	    sdmax = listener;
    else 
        sdmax=socket_server;  

     ret=select(sdmax+1, &read_fds, NULL, NULL, NULL);

    func(sd);

    close(sd);
}