#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 512
#define SERVER_PORT 3003

int main(int argc,char **argv)
{
    int nServerSockFd,nDataSockFd;
    struct sockaddr_in addrClient,addrServer;
    char cRecvBuffer[BUFFER_SIZE];
    int nRcvLen;

    memset(&addrClient, 0, sizeof(addrClient));
    memset(&addrServer, 0, sizeof(addrServer));

    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(SERVER_PORT);

    if(argc>=2)
        addrServer.sin_addr.s_addr = inet_addr(argv[1]);
    else
        addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");

    nServerSockFd = socket(AF_INET,SOCK_STREAM,0);

    if(bind(nServerSockFd,(struct sockaddr *)&addrServer,sizeof(addrServer))==-1)
    {
        printf("Error:connect()\n");
        exit(1);
    }

    if(listen(nServerSockFd,10)==-1)
    {
        printf("Error:listen()\n");
        exit(1);
    }
    socklen_t sLen=sizeof(addrClient);
    nDataSockFd = accept(nServerSockFd,(struct sockaddr *)&addrClient,&sLen);
  
    if(nDataSockFd==-1)
    {
        printf("Error:acept()\n");
        exit(1);
    }
  
    for(;;)
    {
        memset(cRecvBuffer,0,BUFFER_SIZE);
        nRcvLen=read(nDataSockFd,cRecvBuffer,BUFFER_SIZE);
        if(write(nDataSockFd,cRecvBuffer,nRcvLen)==-1)
        {
            printf("Error:acept()\n");
            exit(1);
        }
    }

    close(nDataSockFd);
    close(nServerSockFd);
    return 0;

}
