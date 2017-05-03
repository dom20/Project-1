*/Project #1 Server
the server sets up a socket and keeps waiting for the client to send the query 
in an infinite loop
the server when receiving the file name
opens the file and sends its details back to the same address after being forked/*
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define MAXSIZE 1024

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;//sockfd is the socket descriptor
    char recvBuff[1024]; // buffer for recieving data
    struct sockaddr_in serv_addr; // socket address

    struct hostent *he; //hostent official name of host,aliases and address lsit
            
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)// make a socket
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    /* resolve hostname */
    if ( (he = gethostbyname(argv[1]) ) == NULL ) {//argv[1] is the hostname provided by the server
      exit(1); /* error */
    }
     int port_num = atoi(argv[2]);// set port number 
    /* copy the network address to sockaddr_in structure */
    memcpy(&serv_addr.sin_addr, he->h_addr_list[0], he->h_length);
    /*set the sock variables*/
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_num);

    /* and now  you can connect */
    if ( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) ){
        perror("unable to connect : ");
        exit(1); /* error */
    }

    memset(recvBuff, '0',sizeof(recvBuff));//clear the recving buffer

    char buffer[1024];
    /*prompt user for file name*/
    printf("Client: enter the file name \n");
    fgets(buffer,MAXSIZE-1,stdin);//read user input
    /*sending the file name*/
    if ((send(sockfd,buffer, strlen(buffer),0))== -1) {
            perror("Server not responding:");
            close(sockfd);
            exit(1);
    }
    /*read socket for file from server*/
    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    } 

    if(n < 0)
    {
        printf("\n Read error \n");
    } 
    close(sockfd);//close the socket
    return 0;
}   


