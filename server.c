#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(){

    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];

    //Clean buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    //set port and ip
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind to the port and ip
    if(bind(socket_desc, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
        printf("Error binding server\n");
        return -1;
    }
    printf("Done with binding\n");

    //listen for clients
    if(listen(socket_desc, 1) < 0){
        printf("Error while listening\n");
        return -1;
    }
    printf("Listening for incoming connections...\n");

    //Accept incoming connection
    client_size = sizeof(client_addr);
    client_sock = accept(socket_desc, (struct sockaddr*) &client_addr, &client_size);

    if(client_sock < 0){
        printf("Can't accept\n");
        return -1;
    }
    printf("Client connected at IP: %i and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    //Recieve clients message
    if(recv(client_sock, client_message, sizeof(client_message), 0) < 0){
        printf("Couldn't recieve\n");
        return -1;
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