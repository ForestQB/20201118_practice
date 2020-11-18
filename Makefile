#make client
#./tcpEchoClient
#make server
#./tcpEchoServer
client: 20201118_TCP_Client.c
	gcc -o tcpEchoClient 20201118_TCP_Client.c
server: 20201118_TCP_Server.c
	gcc -o tcpEchoServer 20201118_TCP_Server.c
