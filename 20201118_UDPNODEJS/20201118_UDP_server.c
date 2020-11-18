#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 512
#define SERVER_PORT 3001

int main(int argc,char **argv)
{
	int nServerSockFd;
	struct sockaddr_in addrServer, addrClient;
	socklen_t nLen;

	char cRecvBuffer[BUFFER_SIZE];


	memset(&addrClient,0,sizeof(addrClient));
	memset(&addrServer,0,sizeof(addrServer));

	addrServer.sin_family = AF_INET;
	addrServer.sin_port=htons(SERVER_PORT);
	//addrServer.sin_port=SERVER_PORT;//客戶端跟 server端要一致

	if(argc>=2)
		addrServer.sin_addr.s_addr=inet_addr(argv[1]);
	else
		addrServer.sin_addr.s_addr=inet_addr("127.0.0.1");
	

	nServerSockFd = socket(AF_INET,SOCK_DGRAM,0);

	bind(nServerSockFd,(struct sockaddr *)&addrServer,sizeof(addrServer));
	
	for(;;)
	{
		int nReadBytes = recvfrom(nServerSockFd,cRecvBuffer,
			sizeof(cRecvBuffer),0,(struct sockaddr *)&addrClient,&nLen);
		sendto(nServerSockFd, cRecvBuffer,nReadBytes,
			0,(struct sockaddr *)&addrClient,nLen);

		
	}
	return 0;

}