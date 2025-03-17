server

#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int welcome, new_soc, fd, n;
    char buffer[1024], fname[50];
    struct sockaddr_in addr = {AF_INET, htons(7891), inet_addr("127.0.0.1")};

    welcome = socket(PF_INET, SOCK_STREAM, 0);
    bind(welcome, (struct sockaddr *)&addr, sizeof(addr));
    listen(welcome, 5);
    
    new_soc = accept(welcome, NULL, NULL);
    recv(new_soc, fname, sizeof(fname), 0);

    fd = open(fname, O_RDONLY);
    if (fd < 0) {
        send(new_soc, "File not found", 14, 0);
    } else {
        while ((n = read(fd, buffer, sizeof(buffer))) > 0)
            send(new_soc, buffer, n, 0);
        close(fd);
    }

    return 0;
}


client

#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int soc, n;
    char buffer[1024], fname[50];
    struct sockaddr_in addr = {AF_INET, htons(7891), inet_addr("127.0.0.1")};

    soc = socket(PF_INET, SOCK_STREAM, 0);
    while (connect(soc, (struct sockaddr *)&addr, sizeof(addr))) ;
    printf("\nClient connected to Server\nEnter file name: ");
    scanf("%s", fname);

    send(soc, fname, sizeof(fname), 0);

    while ((n = recv(soc, buffer, sizeof(buffer), 0)) > 0)
        printf("%.*s", n, buffer);

    return 0;
}
