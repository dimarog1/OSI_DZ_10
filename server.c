#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int serverSocket, clientSocket[2], clientLength, receivedSize;
    struct sockaddr_in serverDetails, clientDetails[2];
    char clientMessage[1024];

    // Server socket creation
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        fprintf(stderr, "Error: Unable to create socket\n");
        return EXIT_FAILURE;
    }

    // Server details setup
    serverDetails.sin_family = AF_INET;
    serverDetails.sin_addr.s_addr = INADDR_ANY;
    serverDetails.sin_port = htons(atoi(argv[2]));

    // Binding the socket to the address
    if (bind(serverSocket, (struct sockaddr *)&serverDetails, sizeof(serverDetails)) < 0)
    {
        perror("Error: Binding failed");
        return EXIT_FAILURE;
    }

    // Setting the socket to listen mode
    listen(serverSocket, 3);

    struct sockaddr_in socketDetails;
    socklen_t length = sizeof(socketDetails);
    if (getsockname(serverSocket, (struct sockaddr *)&socketDetails, &length) == -1)
    {
        perror("Error: getsockname failed");
        return EXIT_FAILURE;
    }
    else
    {
        printf("Server is now listening on IP: %s, port: %d\n", inet_ntoa(socketDetails.sin_addr), ntohs(socketDetails.sin_port));
    }

    printf("Server is waiting for incoming connections...\n");
    clientLength = sizeof(struct sockaddr_in);

    for (int i = 0; i < 2; ++i)
    {
        clientSocket[i] = accept(serverSocket, (struct sockaddr *)&clientDetails[i], (socklen_t *)&clientLength);
        if (clientSocket[i] < 0)
        {
            perror("Error: Acceptance failed");
            return EXIT_FAILURE;
        }
        printf("Server has accepted a connection from %s:%d\n", inet_ntoa(clientDetails[i].sin_addr), ntohs(clientDetails[i].sin_port));
    }

    while ((receivedSize = recv(clientSocket[0], clientMessage, 1024, 0)) > 0)
    {
        if (strcmp(clientMessage, "The End") == 0)
        {
            break;
        }
        write(clientSocket[1], clientMessage, strlen(clientMessage));
        memset(clientMessage, 0, 1024); // Buffer cleanup
    }

    // Handling connection closure or error
    if (receivedSize == 0)
    {
        printf("A client has disconnected\n");
    }
    else if (receivedSize == -1)
    {
        perror("Error: Receiving failed");
    }

    return EXIT_SUCCESS;
}