
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

    write(socketFileDescriptor, &ch, 1);
    printf("\n----------------------------------------------------------\n");
    connection_handler(socketFileDescriptor);
    close(socketFileDescriptor);
}
