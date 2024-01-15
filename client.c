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

int port = 7000;

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 1, ret;
    char recvBuff[1024], sendBuff[1024];
    struct sockaddr_in serv_addr; 
    socklen_t addrlen;

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    ret = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &n,sizeof(n));
    if(ret < 0)
    {
        printf("setsockopt failed !\n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 
    addrlen = sizeof(serv_addr);
    while(1)
    {
        printf("please input messgae: ");
	    scanf("%s", sendBuff);
        printf("send: %s\n", sendBuff);
        ret = sendto(sockfd, sendBuff,strlen(sendBuff),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
        if(ret < 0)
        {
            printf("boardcast failed !\n");
            return 1;
        }
        n = recvfrom(sockfd, &recvBuff, sizeof(recvBuff), 0, (struct sockaddr*)&serv_addr, (socklen_t *)&addrlen);
        // n = read(connfd, recvBuff, sizeof(recvBuff)-1);
        if(n < 0)
        {
            printf("\n client close connection. \n");
            break;
        }
        recvBuff[n] = 0;
        printf("recv: %d bytes %s\n", n, recvBuff);
        // n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
        // if(n <= 0)
        // {
        //     printf("\n Read error \n");
	    //     break;
    	// } 
	    // recvBuff[n] = 0;
        // printf("recv: %d bytes %s\n", n, recvBuff);
    }
    close(sockfd);
    return 0;
}
