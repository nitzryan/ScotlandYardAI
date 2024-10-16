#ifndef MODELSOCKET_H
#define MODELSOCKET_H

#include <vector>
#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>


class ModelSocket
{
public:
    ModelSocket();
    ~ModelSocket();
    float TrainModel(std::vector<std::vector<std::vector<unsigned char>>> gameMapPred, std::vector<std::vector<unsigned char>> fugitiveLocations, std::vector<bool> fugitivesWon, std::string name);
    // GameMove GetFugitiveMove(std::vector<unsigned char> snapshot);
    // std::vector<GameMove> GetDetectiveMoves(std::vector<unsigned char> snapshot);
private:
    WSADATA wsaData;
    addrinfo hints;
    SOCKET connectSocket;
};

#endif // MODELSOCKET_H
