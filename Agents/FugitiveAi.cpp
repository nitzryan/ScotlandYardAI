#include "FugitiveAi.h"
#include <random>

FugitiveAi::FugitiveAi() {}

GameMove FugitiveAi::GetMove(const MoveGenerator *moveGenerator, const GameSnapshot &snapshot)
{
    std::vector<GameMove> possibleMoves = moveGenerator->GetPossibleMoves(
        snapshot.fugSquare,
        snapshot.fugTaxis,
        snapshot.fugBuses,
        snapshot.fugUndergrounds,
        snapshot.fugMysteries,
        snapshot.fugDoubles,
        {}
        );

    if (possibleMoves.size() == 0) {
        return GameMove{(unsigned char)GameMovesType::FUGITIVE_CANT_MOVE, snapshot.fugSquare};
    }

    std::random_device dev;
    std::mt19937 generator(dev());
    std::uniform_int_distribution<int> r(0, possibleMoves.size() - 1);
    return possibleMoves.at(r(generator));
}
