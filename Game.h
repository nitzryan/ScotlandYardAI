#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Shared.h"

class Game
{
public:
    Game();
    std::vector<GameSnapshot> GetGameStates() const;
    void MakeFugitiveMove(GameMove move);
    void MakeDetectiveMoves(std::vector<GameMove> moves);
    GameSnapshot GetCurrentState() const;
    bool DetectivesWon() const;
    bool FugitiveWon() const;
private:
    std::vector<GameSnapshot> gameStates;
    GameSnapshot currentState;
    bool detectivesWon, fugitiveWon;
};

#endif // GAME_H
