#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define CONNECTION_PORT 3500
#define IP_ADDR "192.168.4.43"

int main(){

    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    int option_value = 1;

    //Clean buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_desc < 0){
        perror("Error while creating socket\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n");

    if(setsockopt(socket_desc, SOL_SOCKET,SO_REUSEADDR , &option_value,sizeof(option_value)) < 0){
        perror("Couldn't set options\n");
        exit(EXIT_FAILURE);
    }

    //set port and ip
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(CONNECTION_PORT);
    //inet_addr(IP_ADDR)
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_zero[8] = '\0';

    // Bind to the port and ip
    if(bind(socket_desc, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
        perror("Error binding server\n");
        exit(EXIT_FAILURE);
    }
    printf("Done with binding\n");

    //listen for clients
    if(listen(socket_desc, 4) < 0){
        perror("Error while listening\n");
        exit(EXIT_FAILURE);
    }
    printf("Listening for incoming connections...\n");

    //Accept incoming connection
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*) &client_addr, &client_size);

    if(client_sock < 0){
        perror("Can't accept\n");
        exit(EXIT_FAILURE);
    }
    printf("Client connected at IP: %i and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    //Recieve clients message
    if(recv(client_sock, client_message, sizeof(client_message), 0) < 0){
        perror("Couldn't recieve\n");
        exit(EXIT_FAILURE);
    }
    printf("Msg from client: %s\n", client_message);

    //Respond to client
    strcpy(server_message, "This is the server's message.");
    if(send(client_sock, server_message, sizeof(server_message), 0) < 0){
        printf("Couldn't send message\n");
    }
    printf("Server message sent\n");

    //Closing the sockets
    close(client_sock);
    close(socket_desc);

    return 0;
}