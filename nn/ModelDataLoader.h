#ifndef MODELDATALOADER_H
#define MODELDATALOADER_H

#include "Shared.h"
#include <vector>
#include <string>

class ModelSocket;

class ModelDataLoader
{
public:
    ModelDataLoader();
    ~ModelDataLoader();
    float Train(std::vector<std::vector<GameSnapshot>> games, std::vector<bool> fugitivesWon, std::string name) const;
    std::vector<float> GetTileProbabilities(std::vector<GameSnapshot> gameSnapshot, std::string name) const;
    // GameMove GetFugitiveMove(const GameSnapshot& snapshot) const;
    // std::vector<GameMove> GetDetectiveMoves(const GameSnapshot& snapshot) const;
private:
    ModelSocket* socket;
};

#endif // MODELDATALOADER_H
