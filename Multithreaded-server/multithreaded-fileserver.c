#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define on_error(...)                 \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
        perror("cause by");           \
        fflush(stderr);               \
        exit(1);                      \
    }

void *echo(void *arg)
{
    int connfd = *(int *)arg;
    int data_size = 0;
    int bytesReceived = 0;
    char dataSizeBuffer[4];
    char recvBuff[512];
    long value = 0;

    data_size = recv(connfd, dataSizeBuffer, sizeof(dataSizeBuffer), 0);
    printf("Data-name length: %s \n", dataSizeBuffer);
    char *ptr;
    value = strtol(dataSizeBuffer, &ptr, 10);
    printf("Value: %ld \n", value);
    char dataNameBuffer[value];
    data_size = recv(connfd, dataNameBuffer, value, 0);
    printf("Data-name: %s \n", dataNameBuffer);

    FILE *fp;
    fp = fopen(dataNameBuffer, "wb");
    if (NULL == fp)
    {
        printf("Error opening file");
        return 1;
    }

    /* Receive data in chunks of 512 bytes */
    while ((bytesReceived = recv(connfd, recvBuff, sizeof(recvBuff), 0)) > 0)
    {

        fwrite(recvBuff, 1, bytesReceived, fp);
    }

    if (bytesReceived == 0)
    {
        puts("Client disconnected");
    }
    else if (bytesReceived == -1)
    {
        perror("recv failed");
    }
    close(connfd);
    fclose(fp);
    return arg;
}

int main(int argc, char *argv[])
{
    /*
     * check command line arguments
     */
    // if (argc < 2)
    //     on_error("Usage: %s [port]\n", argv[0]);
    int port = 8888;

    int server_fd, client_fd;                  /* server and client file descriptor*/
    struct sockaddr_in serveraddr, clientaddr; /* server and client address*/
    socklen_t client_len = sizeof(clientaddr); /* byte size of client's address */

    /*
     * socket: create socket
     */
    server_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        on_error("Could not create socket\n");

    /*
     * build the server's Internet address
     */
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;                /* this is an IPv4Internet address */
    serveraddr.sin_port = htons(port);              /* this is the port we will listen on */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); /* let the system figure out our IP address */

    /*
     * setsockopt: Handy debugging trick that lets
     * us rerun the server immediately after we kill it;
     * otherwise we have to wait about 20 secs.
     * Eliminates "ERROR on binding: Address already in use" error.
     */
    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

    /*
     * bind: associate socket with a port
     */
    if (bind(server_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        on_error("Could not bind socket\n");

    /*
     * listen: make this socket ready to accept connection requests
     * allow 16 requests to queue up
     */
    if (listen(server_fd, 16) < 0)
        on_error("Could not listen on socket\n");

    printf("Server is listening on %d\n", port);

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&clientaddr, &client_len);
        if (client_fd < 0)
            on_error("Could not establish new connection\n");
        printf("Connected: %s:%d, file descriptor: %d\n",
               inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), client_fd);

        /*
         * create child thread to process client
         */
        pthread_t child_tid;
        if (pthread_create(&child_tid, NULL, echo, &client_fd) == 0)
            pthread_detach(child_tid); /* disassociate from parent thread */
        else
            perror("Thread create failed");
    }
    return 0;
}