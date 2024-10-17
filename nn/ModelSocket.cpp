#include "ModelSocket.h"

#include <string>
#include <QDebug>
#include <fstream>

#include "Shared.h"

// #pragma comment(lib, "Ws2_32.lib")

const int PORT=3456;
const int BUFLEN = 256;

ModelSocket::ModelSocket() {
    // Spawn Python Server
    std::string python_init_string = "start cmd /k python3 nn/ModelSocket.py " + std::to_string(PORT);
    int r = std::system(python_init_string.c_str());

    // Connect to Python Server
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

float ModelSocket::TrainModel(std::vector<std::vector<std::vector<unsigned char>> > gameMapPred, std::vector<std::vector<unsigned char>> fugitiveLocations, std::vector<bool> fugitivesWon, std::string name)
{
    // Write data to file
    std::string filename = "nn/models/" + name + ".txt";
    std::ofstream file(filename.c_str(), std::ios::binary);
    if (!file) {
        return 10000000.f;
    }
    for (size_t i = 0; i < gameMapPred.size(); i++) {
        for (size_t j = 0; j < gameMapPred[i].size(); j++) {
            for (auto& c : gameMapPred[i][j]) {
                file << c;
            }
            file << (unsigned char)252;
            file << fugitiveLocations[i][j];
            file << (unsigned char)253;
        }
        file << (unsigned char)254;
    }
    file << (unsigned char)255;
    file.close();

    // Send to server file info
    int iResult;
    std::string request = "train_map," + name;

    if (request.length() > BUFLEN) {
        return 1000000.f;
    }
    const char* sendMsg = request.c_str();
    iResult = send(connectSocket, sendMsg, (int)strlen(sendMsg), 0);
    if (iResult == SOCKET_ERROR) {
        qDebug() << "Send Msg Failed " << WSAGetLastError();
        return 10000.0f;
    }

    // Get back test loss from model
    char recvBuf[BUFLEN];
    iResult = recv(connectSocket, recvBuf, BUFLEN, 0);
    if (iResult > 0) {
        try {
            return std::stof(recvBuf);
        } catch (...) {
            return 1000.0f;
        }
    }

    return 10000.0f;
}

std::vector<float> ModelSocket::GetTileProbabilities(std::vector<std::vector<unsigned char> > gameSnapshotTensor, std::string name)
{
    // This will only be generated on user request, so just use same filename
    // Write data to file
    std::string filename = "nn/models/user_map_pred.txt";
    std::ofstream file(filename.c_str(), std::ios::binary);
    if (!file) {
        return {};
    }
    for (size_t i = 0; i < gameSnapshotTensor.size(); i++) {
        for (auto& c : gameSnapshotTensor[i]) {
            file << c;
        }
        file << (unsigned char)254;
    }
    file << (unsigned char)255;
    file.close();

    // Send to server file info
    int iResult;
    std::string request = "get_map," + name;
    if (request.length() > BUFLEN) {
        return {};
    }
    const char* sendMsg = request.c_str();
    iResult = send(connectSocket, sendMsg, (int)strlen(sendMsg), 0);
    if (iResult == SOCKET_ERROR) {
        qDebug() << "Send Msg Failed " << WSAGetLastError();
        return {};
    }

    // Get back test loss from model
    char recvBuf[BUFLEN];
    std::vector<float> probabilities;
    probabilities.reserve(NUM_POINTS);
    while (true) {
        iResult = recv(connectSocket, recvBuf, BUFLEN, 0);
        if (iResult > 0) {
            try {
                float prob = std::stof(recvBuf);
                probabilities.push_back(prob);
            } catch (...) {
                return probabilities;
            }
        }
    }


    return {};
}
