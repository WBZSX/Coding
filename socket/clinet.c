#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>

#define PORT 6000


int main()
{
    int socketfd, error;
    struct sockaddr_in serv_addr;
    
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    error = connect(socketfd, &serv_addr, sizeof(struct sockaddr));

    if(0 != error)
    {
        printf("connect server error.");
        return 0;
    }

    char *buff = "this is a test";
    error = send(socketfd, buff, strlen(buff), 0);
    if(-1 == error)
    {
        printf("send message to server error.");
        return 0;
    }

    char recv_buff[1024] = {0};
    error = recv(socketfd, recv_buff, 1024,0);
    if(-1 == error)
    {
	printf("receive error");
	return 0;
    }
    printf("Receive buff:%s\n", recv_buff);

    close(socketfd);
    return 0;
}