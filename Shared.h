#ifndef SHARED_H
#define SHARED_H

#include <cstddef>

enum class TRAVEL_TYPE {
    TAXI,
    BUS,
    UNDERGROUND,
    FERRY
};

enum class DRAW_OBJ {
    STATION,
    TAXI,
    BUS,
    UNDERGROUND,
    FERRY
};

struct matrixVal {
    unsigned char row;
    unsigned char col;
};

const std::size_t NUM_DETECTIVES = 5;

struct GameSnapshot {
    unsigned char turn;
    unsigned char detSquare[NUM_DETECTIVES];
    unsigned char detTaxis[NUM_DETECTIVES];
    unsigned char detBuses[NUM_DETECTIVES];
    unsigned char detUndergrounds[NUM_DETECTIVES];
    unsigned char fugSquare;
    unsigned char fugTaxis;
    unsigned char fugBuses;
    unsigned char fugUndergrounds;
    unsigned char fugDoubles;
    unsigned char fugMysteries;
    unsigned char fugMove;
    bool isFugitiveMove;
    bool wasFugitiveMove;

    // The equality operator assumes they are from the same gamee
    bool operator==(const GameSnapshot& b) {
        return turn == b.turn
               && (isFugitiveMove == b.isFugitiveMove);
    }
};

enum class GameMovesType : unsigned char {
    TAXI=1,
    BUS=2,
    UNDERGROUND=4,
    MYSTERY=8,
    DOUBLE=16,
    DETECTIVE_CANT_MOVE=32,
    FUGITIVE_CANT_MOVE=64
};

struct GameMove {
    unsigned char type;
    unsigned char space;
};

const unsigned char NUM_POINTS = 199;
const unsigned char REVEAL_TURN_1 = 8;
const unsigned char REVEAL_TURN_2 = 13;
const unsigned char REVEAL_TURN_3 = 18;
const unsigned char FINAL_TURN = 24;

#endif // SHARED_H
