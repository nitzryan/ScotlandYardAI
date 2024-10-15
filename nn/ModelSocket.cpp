#include "ModelSocket.h"

#include <stdio.h>
#include <string>
#include <QDebug>

// #pragma comment(lib, "Ws2_32.lib")

const int PORT=3456;
const int BUFLEN = 256;

ModelSocket::ModelSocket() {
    connectSocket = INVALID_SOCKET;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        qDebug() << "WSASTartup Failed " << iResult;
        exit(1);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo* result = nullptr;
    iResult = getaddrinfo("localhost", std::to_string(PORT).c_str(), &hints, &result);
    if (iResult != 0) {
        qDebug() << "getaddrinfo failed " << iResult;
        WSACleanup();
        exit(1);
    }

    connectSocket = INVALID_SOCKET;
    struct addrinfo* ptr = result;
    connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (connectSocket == INVALID_SOCKET) {
        qDebug() << "socket() failed " << WSAGetLastError();
        freeaddrinfo(result);
        return;
    }

    iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
    }
    freeaddrinfo(result);
    if (connectSocket == INVALID_SOCKET) {
        qDebug() << "Unable to connnect to server";
        return;
    }
}

ModelSocket::~ModelSocket()
{
    if (connectSocket != INVALID_SOCKET) {
        closesocket(connectSocket);
    }

    WSACleanup();
}

void ModelSocket::TrainModel(std::vector<std::vector<unsigned char> > gameMapPred, std::vector<bool> fugitivesWon)
{
    // Write data to file

    // Send to server file info
    int iResult;
    const char* sendMsg = "Hello, Server";
    iResult = send(connectSocket, sendMsg, (int)strlen(sendMsg), 0);
    if (iResult == SOCKET_ERROR) {
        qDebug() << "Send Msg Failed " << WSAGetLastError();
        return;
    }

    // Get back test loss from model
    char recvBuf[BUFLEN];
    iResult = recv(connectSocket, recvBuf, BUFLEN, 0);
    if (iResult > 0) {
        qDebug() << recvBuf;
    }
}
