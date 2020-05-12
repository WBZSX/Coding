#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define PORT 6000

int main()
{
    int socketfd, clientfd, error;
    struct sockaddr_in server_addr, client_addr;
    int sin_size;
    char buff[1024] = {0};

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == socketfd)
    {
        printf("create socket fail.");
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    error = bind(socketfd, &server_addr, sizeof(struct sockaddr_in));

    if(0 != error)
    {
        printf("bind fail.");
        close(socketfd);
        return 0;
    }

    error = listen(socketfd, 5);
    if(0 != error)
    {
        printf("listen fail.");
        close(socketfd);
        return 0;
    }

    clientfd = accept(socketfd, (struct sockaddr*)&client_addr, &sin_size);

    error = recv(clientfd, buff, 1024,0);
    if(-1 == error)
    {
        printf("receive error");
        close(socketfd);
        return 0;
    }

    printf("Receive buff:%s\n", buff);

    char *send_buff = "test ok.";
    error = send(clientfd, send_buff, strlen(send_buff), 0);
    if(-1 == error)
    {
        printf("send error");
        close(socketfd);
        return 0;
    }

    close(socketfd);
    

    return 0;
}