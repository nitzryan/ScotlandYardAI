#ifndef MODELDATALOADER_H
#define MODELDATALOADER_H

#include "Shared.h"
#include <vector>

class ModelSocket;

class ModelDataLoader
{
public:
    ModelDataLoader();
    ~ModelDataLoader();
    void Train(std::vector<std::vector<GameSnapshot>> games, std::vector<bool> fugitivesWon) const;
    // GameMove GetFugitiveMove(const GameSnapshot& snapshot) const;
    // std::vector<GameMove> GetDetectiveMoves(const GameSnapshot& snapshot) const;
private:
    ModelSocket* socket;
};

#endif // MODELDATALOADER_H
