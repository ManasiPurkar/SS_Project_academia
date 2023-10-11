



/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address (localhost)

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr) == -1)) {
        perror("Connection failed");
        exit(1);
    }

    while (1) {
        // Send a message to the server
        char message[] = "Hello, Server!";
        send(client_socket, message, sizeof(message), 0);

        // Receive a response from the server
        char buffer[1024];
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("Server Response: %s\n", buffer);

        sleep(1); // Sleep for a second before sending another message
    }

    // Close the client socket
    close(client_socket);

    return 0;
}


void main()
{
    int client_socket;
    struct sockaddr_in server_addr;

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address (localhost)

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr) == -1)) {
        perror("Connection failed");
        exit(1);
    }

    printf("\n**********************************************************\n");
    printf("\n\n*******Welcome to Acadamia********\n");
    printf("\n**********************************************************\n");

    char ch;
 

  
    printf("Available users are 1.Admin(A)  2.Faculty(F)   3.Student(S)\n");
    printf("Enter user type char: \n");
    scanf("%c", &ch);
    if (ch != 'A' && ch != 'F' && ch != 'S') {
        printf("Invalid user type. Please enter A, F, or S.\n");
        exit(0);
   }

    send(client_socket,&ch, sizeof(ch), 0);
    printf("\n----------------------------------------------------------\n");
    connection_handler(socketFileDescriptor);
    close(socketFileDescriptor);
}

*/
