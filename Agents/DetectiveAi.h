#ifndef DETECTIVEAI_H
#define DETECTIVEAI_H

#include "Shared.h"
#include "MoveGenerator.h"
#include <vector>

class DetectiveAi
{
public:
    DetectiveAi();
    std::vector<GameMove> GetMoves(const MoveGenerator* moveGenerator, const GameSnapshot& snapshot);
};

#endif // DETECTIVEAI_H
