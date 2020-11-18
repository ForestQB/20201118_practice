#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

using namespace std;

void * serverthread(void * parm);

pthread_mutex_t mut;

#define SERVER_PORT 2140
#define QLEN 10
#define BUFFER_SIZE

int client_counter = 0;
vector<int> clientSockets;

int main(int argc, const char * argv[])
{
    struct hostent *phost;
    struct protoent *pprotocol;
    struct sockaddr_in serveraddr, clientaddr;
    int sockfd, sockfd2;
    int port;
    int alen;
    pthread_t tid;
    pthread_mutex_init(&mut,NULL);
    memset((char *)&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family =AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    
    if(argc>1)
    {
        port = atoi(argv[1]);
    }
    else
    {
        port = SERVER_PORT;
    }
    if(port>0)
    {
        serveraddr.sin_port = htons((u_short)port);
    }
    else
    {
        fprintf(stderr,"bad port number %s\n",argv[1]);
        exit(1);
    }
    sockfd = socket(PF_INET,SOCK_STREAM, 0);
    if(sockfd<0)
    {
        fprintf(stderr,"socket creation failed\n");
        exit(1);
    }
    if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0)
    {
        fprintf(stderr,"bind failed\n");
        exit(1);
    }
    if(listen(sockfd,QLEN)<0)
    {
        fprintf(stderr,"listen failed\n");
        exit(1);
    }

    alen = sizeof(clientaddr);

    while(1)
    {
        printf("SERVER:Witing for clients...\n");
        if((sockfd2=accept(sockfd,(struct sockaddr*)&clientaddr,&alen))<0)
        {
            fprintf(stderr,"accept failed\n");
            exit(1);
        }
        pthread_create(&tid,NULL,serverthread, (void *) sockfd2);
        for(int i=0;i<clientSockets.size();i++)
        {
            cout << "client sockets:" << clientSockets[i] << endl;
        }
    }
    close(sockfd);
    return 0;
}

void * serverthread(void * parm)
{
    int tsockfd, tclient_counter;
    char buffer[100];
    int nRcvLen=0;
    char cRecvBuffer[BUFFER_SIZE];

    tsockfd = (int) parm;
    clientSockets.push_back(tsockfd);

    pthread_mutex_lock(&mut);
    tclient_counter = ++client_counter;
    pthread_mutex_unlock(&mut);
    sprintf(buffer, "This server has been contacted %d time%s\n",tclient_counter,tclient_counter==1?".":"s.");
    printf("SERVER thread:%s",buffer);

    for(;;)
    {
        memset(cRecvBuffer,0,BUFFER_SIZE);
        nRcvLen = read(tsockfd,cRecvBuffer,BUFFER_SIZE);
        printf("recving %d length\n",nRcvLen);
        if(nRcvLen ==0)
        {
            perror("client socket closed\n");
            break;
        }
        if(nRcvLen ==-1)
        {
            perror("read error!\n");
            break;
        }
        for(int i=0;i<clientSockets.size();i++)
        {
            if(tsockfd !=clientSockets[i])
            {
                sprintf(buffer,"socket id(%d):",tsockfd);
                if(write(clientSockets[i],buffer,strlen(buffer))==-1)
                {
                    perror("write error!\n");
                    break;
                }
                if(write(clientSockets[i],cRecvBuffer,strlen(buffer))==-1)
                {
                    perror("write error!\n");
                    break;
                }
            }
        }
    }
    cout<<"Remove client socket:"<<tsockfd<<endl;
    clientSockets.erase(std::remove(clientSockets.begin(),clientSockets.end(),tsockfd),clientSockets.end());
    close(tsockfd);
    return 0;
}