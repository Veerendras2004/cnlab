server

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serverAddr = {AF_INET, htons(PORT), INADDR_ANY};
    bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("DNS Server is running...\n");

    char domain[BUFFER_SIZE], ip[BUFFER_SIZE];
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    while (1) {
        recvfrom(sockfd, domain, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addrLen);
        struct hostent *host = gethostbyname(domain);
        strcpy(ip, host ? inet_ntoa(*(struct in_addr*)host->h_addr) : "Not found");
        printf("Domain: %s, IP: %s\n", domain, ip);
        sendto(sockfd, ip, strlen(ip), 0, (struct sockaddr*)&clientAddr, addrLen);
    }
    close(sockfd);
}



client
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serverAddr = {AF_INET, htons(PORT)};
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    char domain[BUFFER_SIZE], ip[BUFFER_SIZE];
    printf("Enter domain: ");
    scanf("%s", domain);

    sendto(sockfd, domain, strlen(domain), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    recvfrom(sockfd, ip, BUFFER_SIZE, 0, NULL, NULL);
    printf("IP Address: %s\n", ip);
    close(sockfd);
}
