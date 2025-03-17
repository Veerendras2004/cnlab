server

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int sock, bytes_received, connected, reuse = 1, i = 1;
    char send_data[1024], data[1024], fr[30] = " ";
    struct sockaddr_in server_addr, client_addr;
    socklen_t sin_size;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket not created");
        exit(1);
    }

    // Set socket options to reuse address
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
        perror("Setsockopt failed");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(17000);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero), 0, 8);

    // Bind socket
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Unable to bind");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sock, 5) == -1) {
        perror("Listen failed");
        exit(1);
    }

    fflush(stdout);
    sin_size = sizeof(struct sockaddr_in);

    // Accept a connection
    connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
    if (connected == -1) {
        perror("Accept failed");
        close(sock);
        exit(1);
    }

    // Communication loop
    while (strcmp(fr, "exit") != 0) {
        printf("Enter Data Frame %d (Enter 'exit' to end): ", i);
        scanf("%s", fr);
        send(connected, fr, strlen(fr), 0);

        bytes_received = recv(connected, data, 1024, 0);
        if (bytes_received > 0) {
            data[bytes_received] = '\0';
            printf("I got an acknowledgment: %s\n", data);
        }
        fflush(stdout);
        i++;
    }

    // Close sockets
    close(connected);
    close(sock);

    return 0;
}



client

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main() {
    int sock, bytes_received, i = 1;
    char receive[30];
    struct hostent *host;
    struct sockaddr_in server_addr;

    host = gethostbyname("127.0.0.1");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket not created");
        exit(1);
    }
    printf("Socket created\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(17000);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    memset(&(server_addr.sin_zero), 0, 8);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Connect");
        exit(1);
    }
    printf("Connected to server\n");

    while (1) {
        bytes_received = recv(sock, receive, 20, 0);
        if (bytes_received <= 0) {
            perror("Receive failed or connection closed");
            break;
        }
        receive[bytes_received] = '\0';

        if (strcmp(receive, "exit") == 0) {
            printf("Server closed the connection\n");
            close(sock);
            break;
        } else {
            if (strlen(receive) < 10) {
                printf("\nFrame %d data %s received\n", i, receive);
                send(sock, "positive", strlen("positive"), 0);
            } else {
                send(sock, "negative", strlen("negative"), 0);
            }
            i++;
        }
    }

    close(sock);
    return 0;
}
