#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "./src/location.h"


void execute_commands(char *command, int client_socket) {
    if (strncmp(command, "create", 6) == 0) {
        if(strstr(command, "location") != NULL) {
            char* location_name = strtok(command, " ");
            location_name = strtok(NULL, " ");  // Second token
            location_name = strtok(NULL, " ");  // Third token
            if (location_name != NULL) {
                // For simplicity, assume createLocation returns a string response
                const char* response = createLocation(location_name);
                send(client_socket, response, strlen(response) + 1, 0);
                free((void*)response);  // Free the allocated memory
            } else {
                // Invalid command format
                const char* error_response = "Invalid command format.";
                send(client_socket, error_response, strlen(error_response) + 1, 0);
            }
        }else {
            const char* error = "Unknown entity for creation";
            send(client_socket, error, strlen(error) + 1, 0);
        }
    }else {
        const char* error = "Unknown command";
        send(client_socket, error, strlen(error) + 1, 0);
    }
}

int main()
{

    int server_socket, client_socket;
    struct sockaddr_in server_address;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(2207); // Choose a suitable port

    // Bind the socket to the specified address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1)
    {
        perror("Error listenning for connections");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("server started on port 2207\n");

    printf("Backend service is running...\n");

    while (1)
    {

        // Accept a connection from a client
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0)
        {
            perror("Error accepting connection");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        printf("Client connected\n");

        // Handle communication with the client (e.g., receive commands and send responses)
        char command[1024];
        ssize_t received_bytes;

        while (1)
        {
            received_bytes = recv(client_socket, command, sizeof(command), 0);
            if (received_bytes <= 0)
            {
                perror("Error receiving data \n");
                break;
            }

            command[received_bytes] = '\0';

            execute_commands(command, client_socket);

            fflush(stdout);
        }

        // Close the client socket
        close(client_socket);
        printf("Client disconnected\n");
    }

    // Close the server socket
    close(server_socket);

    return 0;
}