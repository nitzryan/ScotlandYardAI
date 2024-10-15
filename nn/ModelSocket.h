#ifndef MODELSOCKET_H
#define MODELSOCKET_H

#include <vector>
#include "Shared.h"

#include <winsock2.h>
#include <ws2tcpip.h>


class ModelSocket
{
public:
    ModelSocket();
    ~ModelSocket();
    void TrainModel(std::vector<std::vector<unsigned char>> gameMapPred, std::vector<bool> fugitivesWon);
    // GameMove GetFugitiveMove(std::vector<unsigned char> snapshot);
    // std::vector<GameMove> GetDetectiveMoves(std::vector<unsigned char> snapshot);
private:
    WSADATA wsaData;
    addrinfo hints;
    SOCKET connectSocket;
};

#endif // MODELSOCKET_H
