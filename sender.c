#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int clientSocket;
    struct sockaddr_in serverDetails;
    char clientMessage[1024];

    if (argc < 3)
    {
        fprintf(stderr, "Error: Incorrect usage. Correct format: %s <server IP> <server port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        perror("Error: Socket creation failed");
    }

    // Setup server parameters
    serverDetails.sin_addr.s_addr = inet_addr(argv[1]);
    serverDetails.sin_family = AF_INET;
    serverDetails.sin_port = htons(atoi(argv[2]));

    // Attempt to connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverDetails, sizeof(serverDetails)) < 0)
    {
        perror("Error: Connection to server failed");
        return EXIT_FAILURE;
    }

    printf("Success: Connected to server\n");

    while (1)
    {
        memset(clientMessage, 0, 1024); // Buffer cleanup
        printf("Please enter your message: ");
        fgets(clientMessage, 1024, stdin);
        if (send(clientSocket, clientMessage, strlen(clientMessage), 0) < 0)
        {
            perror("Error: Message sending failed");
            return EXIT_FAILURE;
        }

        // If message is "The End", break the loop
        if (strncmp(clientMessage, "The End\n", 8) == 0)
        {
            printf("Ending connection as per user request\n");
            break;
        }
        memset(clientMessage, 0, 1024); // Buffer cleanup
    }

    return EXIT_SUCCESS;
}