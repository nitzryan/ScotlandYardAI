#include "Game.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <qdebug>
#include <assert.h>

Game::Game() {
    std::vector<unsigned char> startTileCards = {13, 26, 29, 34, 50, 53, 91, 94, 103, 112, 117, 132, 138, 141, 155, 174, 197, 198};
    unsigned seed = static_cast<unsigned>(std::time(0));
    std::shuffle(startTileCards.begin(), startTileCards.end(), std::default_random_engine(seed));

    currentState.turn = 0;
    for (std::size_t i = 0; i < NUM_DETECTIVES; i++) {
        currentState.detSquare[i] = startTileCards.back();
        startTileCards.pop_back();
        currentState.detTaxis[i] = 10;
        currentState.detBuses[i] = 8;
        currentState.detUndergrounds[i] = 4;
    }

    currentState.fugSquare = startTileCards.back();
    currentState.fugTaxis = 4;
    currentState.fugBuses = 3;
    currentState.fugUndergrounds = 3;
    currentState.fugDoubles = 2;
    currentState.fugMysteries = 5;
    currentState.fugMove = 0;
    currentState.isFugitiveMove = true;

    detectivesWon = false;
    fugitiveWon = false;

    gameStates.push_back(currentState);
}

std::vector<GameSnapshot> Game::GetGameStates() const
{
    return gameStates;
}

bool CheckForDetectiveWin(const GameSnapshot& snapshot) {
    bool result = false;
    for (std::size_t i = 0; i < NUM_DETECTIVES; i++) {
        result |= snapshot.fugSquare == snapshot.detSquare[i];
    }
    return result;
}

bool CheckForFugitiveWin(const GameSnapshot& snapshot) {
    bool result = true;
    for (std::size_t i = 0; i < NUM_DETECTIVES; i++) {
        result &= (snapshot.detTaxis[i] == 0 && snapshot.detBuses[i] == 0 && snapshot.detUndergrounds[i] == 0);
    }
    return result;
}

void Game::MakeFugitiveMove(GameMove move)
{
    currentState.fugMove = move.type;
    currentState.fugSquare = move.space;
    currentState.isFugitiveMove = (move.type & (unsigned char)GameMovesType::DOUBLE) != 0;
    currentState.fugTaxis -= (move.type & (unsigned char)GameMovesType::TAXI) != 0;
    currentState.fugBuses -= (move.type & (unsigned char)GameMovesType::BUS) != 0;
    currentState.fugUndergrounds -= (move.type & (unsigned char)GameMovesType::UNDERGROUND) != 0;
    currentState.fugDoubles -= (move.type & (unsigned char)GameMovesType::DOUBLE) != 0;
    currentState.fugMysteries -= (move.type & (unsigned char)GameMovesType::MYSTERY) != 0;
    currentState.turn++;
    currentState.wasFugitiveMove = true;

    detectivesWon = CheckForDetectiveWin(currentState);
    fugitiveWon = CheckForFugitiveWin(currentState);
    if (move.type == (unsigned char)GameMovesType::FUGITIVE_CANT_MOVE) {
        qDebug() << "I absolutely cannot believe this happend";
        detectivesWon = true;
    }

    gameStates.push_back(currentState);
}

void Game::MakeDetectiveMoves(std::vector<GameMove> moves)
{
    assert(moves.size() == NUM_DETECTIVES);

    currentState.isFugitiveMove = true;
    currentState.fugMove = 0;
    for (std::size_t i = 0; i < NUM_DETECTIVES; i++) {
        int numTaxiMoves = (moves[i].type & (unsigned char)GameMovesType::TAXI) != 0;
        int numBusMoves = (moves[i].type & (unsigned char)GameMovesType::BUS) != 0;
        int numUndergroundMoves = (moves[i].type & (unsigned char)GameMovesType::UNDERGROUND) != 0;
        currentState.detTaxis[i] -= numTaxiMoves;
        currentState.detUndergrounds[i] -= numUndergroundMoves;
        currentState.detBuses[i] -= numBusMoves;

        currentState.fugTaxis += numTaxiMoves;
        currentState.fugBuses += numBusMoves;
        currentState.fugUndergrounds += numUndergroundMoves;

        currentState.detSquare[i] = moves[i].space;
        if (moves[i].type == (unsigned char)GameMovesType::DETECTIVE_CANT_MOVE) {
            currentState.detTaxis[i] = 0;
            currentState.detBuses[i] = 0;
            currentState.detUndergrounds[i] = 0;
        }
    }

    detectivesWon = CheckForDetectiveWin(currentState);
    fugitiveWon = CheckForFugitiveWin(currentState);
    currentState.wasFugitiveMove = false;

    gameStates.push_back(currentState);
}


GameSnapshot Game::GetCurrentState() const
{
    return currentState;
}

bool Game::DetectivesWon() const
{
    return detectivesWon;
}

bool Game::FugitiveWon() const
{
    return fugitiveWon;
}
