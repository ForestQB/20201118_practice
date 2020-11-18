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
    int nSockFd;
    struct sockaddr_in addrClient,addrServer;
    socklen_t sLen;
    char cSendBuffer[BUFFER_SIZE],cRecvBuffer[BUFFER_SIZE];
    int nRcvLen;

    memset(&addrClient, 0 ,sizeof(addrClient));
    memset(&addrServer, 0,sizeof(addrServer));

    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(SERVER_PORT);

    if(argc>=2)
        addrServer.sin_addr.s_addr = inet_addr(argv[1]);
    else
        addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");

    nSockFd = socket(AF_INET,SOCK_STREAM,0);

    if(connect(nSockFd,(struct sockaddr *)&addrServer,sizeof(addrServer))==-1)
    {
        printf("Error:connect()\n");
        exit(1);
    }

    for(;;)
    {
        printf("Please Input Mesage:");
        fgets(cSendBuffer,BUFFER_SIZE,stdin);

        if(strncmp(cSendBuffer,"exit",4)==0) break;

        write(nSockFd,cSendBuffer,BUFFER_SIZE);

        int nLen= read(nSockFd,cRecvBuffer,BUFFER_SIZE);

        printf("Recvive %d bytes\n",nLen);
        printf("Echo:%s\n",cRecvBuffer);
    }

    close(nSockFd);
    return 0;

}
