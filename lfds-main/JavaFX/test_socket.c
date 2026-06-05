#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT1 12345
#define PORT2 12345

void sendValues() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serverAddr.sin_port = htons(PORT1);

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= 100; i += 10) {
        char buffer[10];
        sprintf(buffer, "%d\n", i);
        write(sock, buffer, sizeof(buffer));
        sleep(1);
    }

    close(sock);
}

void receiveValues() {
    int listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serverAddr.sin_port = htons(PORT2);

    if (bind(listenSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(listenSock, 1) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    int acceptSock = accept(listenSock, NULL, NULL);
    if (acceptSock < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    char buffer[10];
    while (1) {
        ssize_t bytesRead = read(acceptSock, buffer, sizeof(buffer));
        if (bytesRead <= 0) {
            break;
        }
        printf("Received: %s", buffer);
    }

    close(acceptSock);
    close(listenSock);
}

int main() {
    // Erstelle den Thread zum Senden der Werte
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Kindprozess: Sende Werte
        sendValues();
    } else {
        // Elternprozess: Empfange Werte
        receiveValues();
    }

    return 0;
}