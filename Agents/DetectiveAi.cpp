#include "DetectiveAi.h"
#include <random>

DetectiveAi::DetectiveAi() {}

std::vector<GameMove> DetectiveAi::GetMoves(const MoveGenerator *moveGenerator, const GameSnapshot &snapshot)
{
    std::vector<GameMove> allDetectiveMoves;
    allDetectiveMoves.reserve(NUM_DETECTIVES);
    std::random_device dev;
    std::mt19937 generator(dev());

    std::vector<unsigned char> ignoreTiles;
    for (size_t i = 0; i < NUM_DETECTIVES; i++) {
        ignoreTiles.push_back(snapshot.detSquare[i]);
    }

    for (size_t i = 0; i < NUM_DETECTIVES; i++) {
        std::vector<GameMove> possibleMoves = moveGenerator->GetPossibleMoves(
            snapshot.detSquare[i],
            snapshot.detTaxis[i],
            snapshot.detBuses[i],
            snapshot.detUndergrounds[i],
            0,
            0,
            ignoreTiles
            );


        if (possibleMoves.size() == 0) {
            allDetectiveMoves.push_back(GameMove{(unsigned char)GameMovesType::DETECTIVE_CANT_MOVE, snapshot.detSquare[i]});
            continue;
        }
        std::uniform_int_distribution<size_t> r(0, possibleMoves.size() - 1);
        allDetectiveMoves.push_back(possibleMoves.at(r(generator)));
    }

    return allDetectiveMoves;
}
