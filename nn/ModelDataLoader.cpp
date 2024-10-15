#include "ModelDataLoader.h"
#include "ModelSocket.h"
#include <assert.h>

const int MAP_PRED_FUG_SIZE = 8;
const int MAP_PRED_DET_SIZE = 4;
const int MAP_PRED_SIZE = MAP_PRED_FUG_SIZE + NUM_DETECTIVES * MAP_PRED_DET_SIZE;

std::vector<unsigned char> GenerateMapPredInput(const GameSnapshot& snapshot) {
    std::vector<unsigned char> mapInputData(MAP_PRED_SIZE);

    mapInputData[0] = snapshot.turn;
    mapInputData[1] = snapshot.fugMove & ~(unsigned char)GameMovesType::DOUBLE;
    mapInputData[2] = snapshot.isFugitiveMove;
    mapInputData[3] = snapshot.fugTaxis;
    mapInputData[4] = snapshot.fugBuses;
    mapInputData[5] = snapshot.fugUndergrounds;
    mapInputData[6] = snapshot.fugMysteries;
    mapInputData[7] = snapshot.fugDoubles;

    for (int i = 0; i < NUM_DETECTIVES; i++) {
        mapInputData[MAP_PRED_FUG_SIZE + MAP_PRED_DET_SIZE * i] = snapshot.detSquare[i];
        mapInputData[MAP_PRED_FUG_SIZE + MAP_PRED_DET_SIZE * i + 1] = snapshot.detTaxis[i];
        mapInputData[MAP_PRED_FUG_SIZE + MAP_PRED_DET_SIZE * i + 2] = snapshot.detBuses[i];
        mapInputData[MAP_PRED_FUG_SIZE + MAP_PRED_DET_SIZE * i + 3] = snapshot.detUndergrounds[i];
    }

    return mapInputData;
}

ModelDataLoader::ModelDataLoader() {
    socket = new ModelSocket();
}

ModelDataLoader::~ModelDataLoader()
{
    delete socket;
}

void ModelDataLoader::Train(std::vector<std::vector<GameSnapshot> > games, std::vector<bool> fugitivesWon) const
{
    assert(games.size() == fugitivesWon.size());

    std::vector<std::vector<unsigned char>> gameMapSnapshots;
    gameMapSnapshots.reserve(games.size());
    for (int i = 0; i < games.size(); i++) {
        for (int j = 0; j < games[i].size(); j++) {
            std::vector<unsigned char> gameStateMapPred = GenerateMapPredInput(games[i][j]);
            gameMapSnapshots.push_back(gameStateMapPred);
        }
    }

    socket->TrainModel(gameMapSnapshots, fugitivesWon);
}

// GameMove ModelDataLoader::GetFugitiveMove(const GameSnapshot &snapshot) const
// {

// }

// std::vector<GameMove> ModelDataLoader::GetDetectiveMoves(const GameSnapshot &snapshot) const
// {

// }
