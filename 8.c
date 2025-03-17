server
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock, length, fromlen, n;
    struct sockaddr_in server, from;
    char buf[1024];

    // Check if port is provided
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("Opening socket");
    }

    // Configure server address structure
    length = sizeof(server);
    bzero((char *)&server, length);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    // Bind socket to port
    if (bind(sock, (struct sockaddr *)&server, length) < 0) {
        error("Binding");
    }

    // Open file to store received content
    FILE *fp = fopen("received_file.txt", "w");
    if (fp == NULL) {
        error("Error opening file");
    }

    fromlen = sizeof(struct sockaddr_in);
    printf("Server is running on port %s...\n", argv[1]);

    // Receive data in loop
    while (1) {
        n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);
        if (n < 0) {
            error("recvfrom");
        }

        buf[n] = '\0';  // Null-terminate received data
        printf("Received: %s", buf);
        fprintf(fp, "%s", buf);  // Write to file

        // Send acknowledgment to client
        n = sendto(sock, "Received", 8, 0, (struct sockaddr *)&from, fromlen);
        if (n < 0) {
            error("sendto");
        }
    }

    fclose(fp);
    close(sock);
    return 0;
}





client

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock, length, n;
    struct sockaddr_in server;
    struct hostent *hp;
    char buffer[1024];

    // Check for correct arguments
    if (argc != 4) {
        printf("Usage: %s <server> <port> <filename>\n", argv[0]);
        exit(1);
    }

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        error("socket");
    }

    // Configure server address structure
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp == 0) {
        error("Unknown host");
    }
    bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    length = sizeof(struct sockaddr_in);

    // Open file to send
    FILE *fp = fopen(argv[3], "r");
    if (fp == NULL) {
        error("Error opening file");
    }

    // Send file content
    while (fgets(buffer, 1024, fp) != NULL) {
        n = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server, length);
        if (n < 0) {
            error("sendto");
        }

        // Receive acknowledgment
        n = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&server, &length);
        if (n < 0) {
            error("recvfrom");
        }

        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    fclose(fp);
    close(sock);
    return 0;
}
