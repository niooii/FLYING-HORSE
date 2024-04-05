
#include "ServerSocket.h"
#include <thread>

#define MSG_WAITALL 0x8 /* do not complete until packet is completely filled */
#define bzero(p, size) (void) memset((p), 0, (size));

typedef int socklen_t;

ServerSocket::ServerSocket(short PORT) : port{PORT}
{
    struct sockaddr_in serverAddr, clientAddr;
    int i = 0;
    const char* optval = "1";
    int connected = 0;

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
    printf("Listening on port %d\n", port);
    while (true)
    {
        unsigned int clientSocket;
        sockaddr_in clientAddr;
        socklen_t clientLength;

        char name[2048];

        clientLength = sizeof(clientAddr);

        clientSocket = accept(sock, (struct sockaddr*)&clientAddr, &clientLength);
        recv(clientSocket, name, sizeof(name), 0);
        std::thread new_thread(&HandleIncomingConnection, ClientConnectionInfo{
                clientSocket, 
                clientAddr,
                name
            }
        );
        new_thread.detach();
    }
}
