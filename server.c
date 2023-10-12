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

int clients[1000];
int noClients = 0;



void main()
{
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







/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void handle_client(int client_socket) {
    char buffer[256];
    int n;

    bzero(buffer, 256);
    n = read(client_socket, buffer, 255);

    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }

    printf("Received message: %s\n", buffer);

    // Handle client request here as needed

    n = write(client_socket, "Server received your message", 28);

    if (n < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    socklen_t client_length;
    struct sockaddr_in server_address, client_address;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        perror("Error opening socket");
        exit(1);
    }

    bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Error on binding");
        exit(1);
    }

    listen(server_socket, 5);
    client_length = sizeof(client_address);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_length);

        if (client_socket < 0) {
            perror("Error on accept");
            exit(1);
        }

        pid_t child_pid = fork();

        if (child_pid < 0) {
            perror("Fork error");
            exit(1);
        }

        if (child_pid == 0) {
            // This is the child process
            close(server_socket);
            handle_client(client_socket);
            exit(0);
        } else {
            // This is the parent process
            close(client_socket);
        }
    }

    close(server_socket);
    return 0;
}
*/
/*
int main (void)
{
 // Declaration and definition
 int listensd; // Listen socket descriptor
 int connectsd; // Connecting socket descriptor
 int n; // Number of bytes in each reception
 int bytesToRecv; // Total bytes to receive
 int processID; // ID of the child process
 char buffer [256]; // Data buffer
 char* movePtr; // Pointer to the buffer
 struct sockaddr_in serverAddr; // Server address
 struct sockaddr_in clientAddr; // Client address
 int clAddrLen; // Length of client address
 // Create listen socket
 listensd = socket (PF_INET, SOCK_STREAM, 0);
 // Bind listen socket to the local address and port
 memset (&serverAddr, 0, sizeof (serverAddr));
 serverAddr.sin_family = AF_INET;
 serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);
 serverAddr.sin_port = htons (7); // We assume port 7
 bind (listensd, &serverAddr, sizeof (serverAddr));
 // Listen to connection requests
listen (listensd, 5);
 // Handle the connection
 for ( ; ; ) // Run forever
 {
 connectsd = accept (listensd, &clientAddr, &clAddrLen);
 processID = fork ();
 if (processID == 0) // Child process
 {
 close (listensd);
 bytesToRecv = 256;
 movePtr = buffer;
 while ( (n = recv (connectfd, movePtr, bytesToRecv, 0)) > 0)
 {
 movePtr = movePtr + n;
 bytesToRecv = movePtr − n;
 } // End of while
 send (connectsd, buffer, 256, 0);
 exit (0);
 } // End of if
 close (connectsd): // Back to parent process
 } // End of for loop
 } // End of echo server program
 
 */
/*
void handle_client(int client_socket) {
    char buffer[1024];
    int bytes_received;

    while (1) {
        // Receive data from the client
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            perror("Error while reading from client");
            break;
        }

        // Process the received data (you can replace this with your logic)
        buffer[bytes_received] = '\0';
        printf("Received: %s", buffer);

        // Send a response back to the client
        char response[] = "Message received by the server\n";
        send(client_socket, response, sizeof(response), 0);
    }

    close(client_socket);
    exit(0);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Listen for incoming connections
    listen(server_socket, 5);
    printf("Server is listening...\n");

    while (1) {
        // Accept a connection from a client
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Fork a child process to handle the client
        if (fork() == 0) {
            // In the child process
            close(server_socket); // Close the server socket in the child

            // Handle the client's communication
            handle_client(client_socket);
        } else {
            // In the parent process
            close(client_socket); // Close the client socket in the parent
        }
    }

    close(server_socket);
    return 0;
}
*/
