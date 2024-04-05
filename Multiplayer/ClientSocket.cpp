#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
}

void ClientSocket::ConnectTo(const char* ip, unsigned short port)
{
    struct sockaddr_in ServAddr;
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        exit(1);
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&ServAddr, 0, sizeof(ServAddr));
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = inet_addr(ip);
    ServAddr.sin_port = htons(port);

    printf("connection attempted\n");
    while (connect(sock, (struct sockaddr*)&ServAddr, sizeof(ServAddr)) != 0) {
        printf("Connection failed. Error code: %d\n", WSAGetLastError());
        Sleep(10);
    }
    printf("connected\n");
}

void ClientSocket::SendString(std::string str)
{
    // null terminated bullshitt
    send(sock, str.c_str() + '\0', str.size() + 1, 0);
}
