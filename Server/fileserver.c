
// Compile with: $gcc fileserver.c -o fileserver  Run command: $./fileserver
#include <stdio.h>
#include <string.h>     //strlen
#include <sys/socket.h> // socket
#include <arpa/inet.h>  //inet_addr
#include <stdlib.h>

int main(void)
{
    int data_size = 0;
    int listenfd = 0;
    int c;
    int bytesReceived = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr, client;
    char dataSizeBuffer[4];
    char recvBuff[512];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        printf("Could not create socket");
    }

    printf("Socket created\n");

    // Prepare the sockaddr_in structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(8888);

    // Bind
    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        // print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    if (listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }

    /* Create file where data will be stored */
    /*
    FILE *fp;
    fp = fopen("server.dat", "wb");
    if (NULL == fp)
    {
        printf("Error opening file");
        return 1;
    }
    */
    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while (1)
    {
        connfd = accept(listenfd, (struct sockaddr *)&client, (socklen_t *)&c);
        if (connfd < 0)
        {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");
        /*
        FILE *fp;
        fp = fopen("server.dat", "wb");
        if (NULL == fp)
        {
            printf("Error opening file");
            return 1;
        }
        */

        data_size = recv(connfd, dataSizeBuffer, sizeof(dataSizeBuffer), 0);
        printf("Data name length: %s \n", dataSizeBuffer);
        long value;
        char *ptr;
        value = strtol(dataSizeBuffer, &ptr, 10);
        // printf("Value: %ld \n", value);
        char dataNameBuffer[value];
        data_size = recv(connfd, dataNameBuffer, value, 0);
        printf("Data name: %s \n", dataNameBuffer);

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
    }

    return 0;
}