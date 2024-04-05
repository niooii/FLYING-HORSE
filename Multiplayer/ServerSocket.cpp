
#include "ServerSocket.h"

#define MSG_WAITALL 0x8 /* do not complete until packet is completely filled */
#define bzero(p, size) (void) memset((p), 0, (size));

typedef int socklen_t;
void ServerSocket::HandleIncomingConnection()
{
}

ServerSocket::ServerSocket(short PORT)
{
    char buffer[1024];
    char response[18384];
    struct sockaddr_in serverAddr, clientAddr;
    int i = 0;
    const char* optval = "1";
    int connected = 0;
    char tempdir[512];

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, optval, sizeof(optval)) < 0) {
        printf("error setting tcp socket options!\n");
        printf("Error: %d", WSAGetLastError());
        exit(1);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY; // use default
    serverAddr.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        printf("bind failed, error code: %d", WSAGetLastError());
        exit(1);
    }
    if (listen(sock, 5) != 0) {
        printf("listen failed, error code: %d", WSAGetLastError());
        exit(1);
    }
    

    
}

void ServerSocket::Listen() 
{
    while (true)
    {
        unsigned int clientSocket;
        sockaddr_in clientAddr;
        socklen_t clientLength;

        char packet[65536];

        clientLength = sizeof(clientAddr);

        puts("Awating connection...");
        clientSocket = accept(sock, (struct sockaddr*)&clientAddr, &clientLength);
        //printf("Established connection with client %s.\n", inet_nto(clientAddr.sin_addr));
        recv(clientSocket, packet, sizeof(packet), 0);
        printf("packet recieved: %s\n", packet);
        HandleIncomingConnection();
    }
}
