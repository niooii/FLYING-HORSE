
#include "Server.h"
#include <thread>

#define bzero(p, size) (void) memset((p), 0, (size));

typedef int socklen_t;

Server::Server(short PORT) : port{PORT}
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

Server::~Server()
{
    closesocket(sock);
}

void Server::Run() 
{
    printf("Listening on port %d\n", port);
    std::thread replication_thread(&ReplicationLoop);
    while (true)
    {
        unsigned int clientSocket;
        sockaddr_in clientAddr;
        socklen_t clientLength;

        char name[2048];

        clientLength = sizeof(clientAddr);

        clientSocket = accept(sock, (struct sockaddr*)&clientAddr, &clientLength);
        recv(clientSocket, name, sizeof(name), 0);
        auto connection_info = ConnectionInfo {
                clientSocket,
                clientAddr,
        };
        std::thread new_thread(
            &HandleIncomingConnection, 
            connection_info,
            name
        );
        new_thread.detach();
    }
}
