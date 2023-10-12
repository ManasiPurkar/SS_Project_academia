
/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

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
        } 
        else {
            // In the parent process
            close(client_socket); // Close the client socket in the parent
        }
    }

    close(server_socket);
    return 0;
}
*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

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



/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    struct hostent* server;

    char buffer[256];
    sprintf(buffer, "Hello, Server!");

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0) {
        perror("Error opening socket");
        exit(1);
    }

    server = gethostbyname("localhost");

    if (server == NULL) {
        fprintf(stderr, "Error: No such host\n");
        exit(1);
    }

    bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(12345);

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Error connecting");
        exit(1);
    }

    int n = write(client_socket, buffer, strlen(buffer));

    if (n < 0) {
        perror("Error writing to socket");
        exit(1);
    }

    bzero(buffer, 256);
    n = read(client_socket, buffer, 255);

    if (n < 0) {
        perror("Error reading from socket");
        exit(1);
    }

    printf("Server Response: %s\n", buffer);
    close(client_socket);

    return 0;
}

*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>


int main (void)
 {
 // Declaration and definition
 int sd; // Socket descriptor
 int n; // Number of bytes received
 int bytesToRecv; // Number of bytes to receive
 char sendBuffer [256]; // Send buffer
 char recvBuffer [256]; // Receive buffer
 char* movePtr; // A pointer the received buffer
 struct sockaddr_in serverAddr; // Server address

 // Create socket
 sd = socket (PF_INET, SOCK_STREAM, 0);
 // Create server socket address
 memset (&serverAddr, 0, sizeof(serverAddr);
 serverAddr.sin_family = AF_INET;
 inet_pton (AF_INET, “server address”, &serverAddr.sin_addr);
 serverAddr.sin_port = htons (7); // We assume port 7
 connect (sd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
 // Send and receive data
 fgets (sendBuffer, 256, stdin);
 send (fd, sendBuffer, strlen (sendbuffer), 0);
 bytesToRecv = strlen (sendbuffer);
 movePtr = recvBuffer;
 while ( (n = recv (sd, movePtr, bytesToRecv, 0) ) > 0)
 {
 movePtr = movePtr + n;
 bytesToRecv = bytesToRecv − n;
 } // End of while loop
 recvBuffer[bytesToRecv] = 0;
  printf (“Received from server:”);
 fputs (recvBuffer, stdout);
 // Close and exit
 close (sd);
 exit (0);
 }
 
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

        // Send a response back to to the client
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
    printf("client is listening...\n");

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
