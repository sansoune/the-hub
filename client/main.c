#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main()
{

    int client_socket;
    struct sockaddr_in server_address;

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // Connect to localhost
    server_address.sin_port = htons(2207);       // Use the same port as the backend

    // Connect to the backend service
    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    while (1)
    {
        char command[100];
        printf("Alethis> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0)
        {
            break;
        }

        send(client_socket, command, strlen(command), 0);

        char response[1024];
        int bytes_received = recv(client_socket, response, sizeof(response), 0);
        if (bytes_received < 0)
        {
            perror("recv");
            exit(1);
        }

        // Print the response from the server
        printf("%.*s\n", (int)bytes_received, response);
    }

    close(client_socket);
    return 0;
}