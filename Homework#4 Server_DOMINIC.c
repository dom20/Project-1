*/Server Homework# 4
sets up a socket and waits for command from the client
runs the command and sends the output back on the same socket*/
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define BUFLEN 2048
#define PORT 9930
    
void err(char *str)
{
    perror(str);
    exit(1);
}

    
int main(void)
{
    struct sockaddr_in my_addr, cli_addr;
    int sockfd, i;
    socklen_t slen=sizeof(cli_addr);
    char buf[BUFLEN];
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        err("socket");
    else
        printf("Server : Socket() successful\n");
    
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(sockfd, (struct sockaddr* ) &my_addr, sizeof(my_addr))==-1)
        err("bind");
    else
        printf("Server : bind() successful\n");
    
    while(1)
    {
        if (recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen)==-1)
            err("recvfrom()");
        printf("Received packet from %s:%d\nData: %s\n\n",
               inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf);

        FILE *fp;
        char path[2048];

        /* Open the command for reading. */
        fp = popen(buf, "r");
        if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
        }

        /* Read the output a line at a time - output it. */

        while (fgets(path, sizeof(path)-1, fp) != NULL) {
            int n = sendto(sockfd, path, strlen(path), 0, (struct sockaddr*)&cli_addr, slen);
            if( n<0){
                error("Error in sendto");
            }else printf("Sent %d bytes : %s\n",n,path );
        }

        /* close */
        pclose(fp);
    }
    
    close(sockfd);
    return 0;
}
