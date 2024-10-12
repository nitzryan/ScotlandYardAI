#ifndef FUGITIVEAI_H
#define FUGITIVEAI_H

#include "Shared.h"
#include "MoveGenerator.h"

class FugitiveAi
{
public:
    FugitiveAi();
    GameMove GetMove(const MoveGenerator* moveGenerator, const GameSnapshot& snapshot);
};

#endif // FUGITIVEAI_H
