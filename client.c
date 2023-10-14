/*Name- Manasi Purkar
Roll- MT2023158
File- client code
*/


#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#define PORT 8080


void main()
{
    int socketFileDescriptor, connectStatus;
    struct sockaddr_in serverAddress;
    struct sockaddr server;

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1)
    {
        perror("Error while creating server socket!");
        exit(0);
    }

    serverAddress.sin_family = AF_INET;                     // IPv4
    serverAddress.sin_port = htons(PORT);                   // Server will listen to port 8080
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // htonl(INADDR_ANY); // Binds the socket to all interfaces

    connectStatus = connect(socketFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (connectStatus == -1)
    {
        perror("Error while connecting to server!");
        close(socketFileDescriptor);
        exit(0);
    }
   
    char readBuffer[1000],writeBuffer[1000];
    ssize_t readBytes, writeBytes; 
   //bzero(readBuffer, sizeof(readBuffer)); 
  //  bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
    do
    {
   // printf("HELLO");
    bzero(readBuffer, sizeof(readBuffer)); 
    //printf("%s",readBuffer);
    bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
    readBytes = read(socketFileDescriptor, readBuffer, sizeof(readBuffer));
    if (readBytes <= 0)
            perror("Error while reading from client socket!");
   printf("%s", readBuffer);
 //  scanf("%[^\n]%*c", &writeBuffer); // Take user input
   scanf("%s", writeBuffer); // Take user input
  //char username[30];
   //fgets(username,30, stdin);
   //username[strlen(username) - 1] = '\0'; 
   //strcpy(writeBuffer,username);
   // printf("%s",writeBuffer);
    writeBytes=write(socketFileDescriptor,writeBuffer, sizeof(writeBuffer));
    //printf("Hii");
    if (writeBytes == -1)
    {
         perror("Error while writing to client socket!");
         printf("Closing the connection to the server now!\n");
         break;
    }
    }while(readBytes>0);
    
    close(socketFileDescriptor);
}



