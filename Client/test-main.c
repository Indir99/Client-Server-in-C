#include <stdio.h>  //printf
#include <string.h> //strlen
//#include <sys/socket.h> //socket
//#include <arpa/inet.h>  //inet_addr
#include <stdlib.h>

//#define MAX 1

// int main(int argc, char *argv[])
//{
//     char str[] = "data-name";
//     printf("Data: %s \n", str);
//     printf("String len: %ld \n", strlen(str));
//     char c_size = strlen(str) + '0';
//    printf("Char value: %c \n", c_size);
//
//    return 0;
//}

int main(void)
{
    char str[10];
    char *ptr;
    long value;
    strcpy(str, " 123");
    value = strtol(str, &ptr, 10);
    printf("decimal %ld\n", value);

    return 0;
}
