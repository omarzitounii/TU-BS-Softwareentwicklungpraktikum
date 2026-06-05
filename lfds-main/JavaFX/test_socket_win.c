
// in CMake einfügen: target_link_libraries("project_name" ws2_32)

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080

void sendData()
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    int val = 0;

    printf("Sender-Thread gestartet\n");

    // Initialisierung der Winsock-Bibliothek
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Fehler bei der Initialisierung von Winsock\n");
        return;
    }

    // Erstellen des Sockets
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Fehler beim Erstellen des Sockets\n");
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // IP-Adresse des Servers (localhost)
    if (inet_pton(AF_INET, "127.0.0.1", &(server.sin_addr)) <= 0)
    {
        printf("Ungültige Adresse/Adresse nicht unterstützt\n");
        return;
    }

    // Verbindung zum Server herstellen
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Verbindung fehlgeschlagen\n");
        return;
    }

    // Werte von 0 bis 100 in 5er Schritten senden
    while (val <= 100)
    {
        char message[10];
        sprintf(message, "%d", val);
        send(s, message, sizeof(message), 0);
        printf("Sender: Wert %d gesendet\n", val);
        val += 5;
        Sleep(1000); // 1 Sekunde warten
    }

    closesocket(s);
    WSACleanup();

    printf("Sender-Thread beendet\n");
}

void receiveData()
{
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    char *message;

    printf("Empfänger-Thread gestartet\n");

    // Initialisierung der Winsock-Bibliothek
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Fehler bei der Initialisierung von Winsock\n");
        return;
    }

    // Erstellen des Sockets
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Fehler beim Erstellen des Sockets\n");
        return;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Socket an den Port binden
    if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Binden fehlgeschlagen\n");
        return;
    }

    // Auf eingehende Verbindungen warten
    listen(s, 3);

    printf("Empfänger: Warte auf Verbindung...\n");

    c = sizeof(struct sockaddr_in);
    if ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
    {
        printf("Verbindung akzeptiert\n");

        // Daten empfangen und auf der Konsole ausgeben
        while (1)
        {
            char buffer[10] = {0};
            int valread = recv(new_socket, buffer, sizeof(buffer), 0);
            if (valread <= 0)
            {
                break;
            }
            printf("Empfänger: Wert erhalten: %s\n", buffer);
        }
    }

    closesocket(s);
    WSACleanup();

    printf("Empfänger-Thread beendet\n");
}

int main()
{
    // Winsock initialisieren
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Fehler bei der Initialisierung von Winsock\n");
        return 1;
    }

    // Threads für Sender und Empfänger starten
    HANDLE senderThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendData, NULL, 0, NULL);
    HANDLE receiverThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)receiveData, NULL, 0, NULL);

    // Warten, bis beide Threads beendet sind
    WaitForSingleObject(senderThread, INFINITE);
    WaitForSingleObject(receiverThread, INFINITE);

    // Winsock aufräumen
    WSACleanup();

    return 0;
}