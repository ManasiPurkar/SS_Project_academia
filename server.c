#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> //socket functions
#include <netinet/ip.h> // for sockaddr_in stucture
#include <string.h>
#include <stdlib.h>
#define PORT 8080
// includes
#include "login.h"
#include "admin_menu.h"
#include "faculty_menu.h"
#include "student_menu.h"

int clients[1000];
int noClients = 0;



void main()
{
	char readBuffer[1000],writeBuffer[1000];
   	ssize_t readBytes, writeBytes; 
    int socketFileDescriptor, socketBindStatus, socketListenStatus, connfd;
    struct sockaddr_in serverAddress, clientAddress;

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1)
    {
        perror("Error while creating server socket!");
        exit(0);
    }

    serverAddress.sin_family = AF_INET;                // IPv4
    serverAddress.sin_port = htons(PORT);              // Server will listen to port 8080
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Binds the socket to all interfaces

    socketBindStatus = bind(socketFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (socketBindStatus == -1)
    {
        perror("Error while binding to server socket!");
        exit(0);
    }
    printf("Server Listener on port %d \n", PORT);
    socketListenStatus = listen(socketFileDescriptor, 10);
    if (socketListenStatus == -1)
    {
        perror("Error while listening for connections on the server socket!");
        close(socketFileDescriptor);
        exit(0);
    }
    noClients = 1;
    puts("Waiting for connections ...");
    int addrlen;
    while (1)
    {
        addrlen = (int)sizeof(clientAddress);
        connfd = accept(socketFileDescriptor, (struct sockaddr *)&clientAddress, &addrlen);
        if (connfd == -1)
        {
            perror("Error while connecting to client!");
            close(socketFileDescriptor);
            exit(EXIT_FAILURE);
        }

        printf("New connection , socket fd is %d , ip is : %s , port : %d\n", connfd, inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

        if (!fork())
        {
            // Child will enter this branch
            clients[noClients] = connfd;
            int status=login(connfd);
            if(status)
            {
            	if(u.usertype=='A')
    		{
    		int status=admin_menu(connfd);
    		if(status==0)
    		{
    			bzero(writeBuffer, sizeof(writeBuffer));
    			strcpy(writeBuffer,"Admin menu cannot be shown \n");
    			write(connfd, writeBuffer, strlen(writeBuffer));
    		}
    		}
		else if(u.usertype=='F')
    		{
    		int status=faculty_menu(connfd);
    		if(status==0)
    		{
    			bzero(writeBuffer, sizeof(writeBuffer));
    			strcpy(writeBuffer,"Faculty menu cannot be shown \n");
    			write(connfd, writeBuffer, strlen(writeBuffer));
    		}
    		}
    		else if(u.usertype=='S')
    		{
    		int status=student_menu(connfd);
    		if(status==0)
    		{
    			bzero(writeBuffer, sizeof(writeBuffer));
    			strcpy(writeBuffer,"Student menu cannot be shown \n");
    			write(connfd, writeBuffer, strlen(writeBuffer));
    		}
    		}
            }
            close(connfd);
            exit(0);
        }
        else
        {
            noClients++;
        }
    }

    close(socketFileDescriptor);
}







