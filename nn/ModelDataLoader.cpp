#include "ModelDataLoader.h"
#include "ModelSocket.h"
#include <assert.h>

// const int MAP_PRED_FUG_SIZE = 9;
// const int MAP_PRED_DET_SIZE = 4;
const int MAP_PRED_FUG_SIZE = 4;
const int MAP_PRED_DET_SIZE = 1;
const int MAP_PRED_SIZE = MAP_PRED_FUG_SIZE + NUM_DETECTIVES * MAP_PRED_DET_SIZE;

std::vector<unsigned char> GenerateMapPredInput(const GameSnapshot& snapshot) {
    std::vector<unsigned char> mapInputData(MAP_PRED_SIZE);

    mapInputData[0] = snapshot.turn;
    mapInputData[1] = snapshot.fugMove & ~(unsigned char)GameMovesType::DOUBLE;
    mapInputData[2] = snapshot.isFugitiveMove;
    //mapInputData[3] = snapshot.fugTaxis;
    //mapInputData[4] = snapshot.fugBuses;
    //mapInputData[5] = snapshot.fugUndergrounds;
    //mapInputData[6] = snapshot.fugMysteries;
    //mapInputData[7] = snapshot.fugDoubles;

    if (snapshot.turn == REVEAL_TURN_1 ||
        snapshot.turn == REVEAL_TURN_2 ||
        snapshot.turn == REVEAL_TURN_3 ||
        snapshot.turn == FINAL_TURN ||
        snapshot.fugSquare == snapshot.detSquare[0] ||
        snapshot.fugSquare == snapshot.detSquare[1] ||
        snapshot.fugSquare == snapshot.detSquare[2] ||
        snapshot.fugSquare == snapshot.detSquare[3] ||
        snapshot.fugSquare == snapshot.detSquare[4]) {

        mapInputData[3] = snapshot.fugSquare;
    } else {
        mapInputData[3] = 240;
    }

    for (int i = 0; i < NUM_DETECTIVES; i++) {
        mapInputData[MAP_PRED_FUG_SIZE + MAP_PRED_DET_SIZE * i] = snapshot.detSquare[i];
        //mapInputData[MAP_PRED_FUG_SIZE + MAP_PRED_DET_SIZE * i + 1] = snapshot.detTaxis[i];
        //mapInputData[MAP_PRED_FUG_SIZE + MAP_PRED_DET_SIZE * i + 2] = snapshot.detBuses[i];
        //mapInputData[MAP_PRED_FUG_SIZE + MAP_PRED_DET_SIZE * i + 3] = snapshot.detUndergrounds[i];
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

float ModelDataLoader::Train(std::vector<std::vector<GameSnapshot> > games, std::vector<bool> fugitivesWon, std::string name) const
{
    assert(games.size() == fugitivesWon.size());

    std::vector<std::vector<std::vector<unsigned char>>> gameMapSnapshots;
    gameMapSnapshots.reserve(games.size());
    std::vector<std::vector<unsigned char>> fugitiveLocations;
    fugitiveLocations.reserve(games.size());

    for (int i = 0; i < games.size(); i++) {
        std::vector<std::vector<unsigned char>> singleGameSnapshots;
        std::vector<unsigned char> singleGameLocations;
        for (int j = 0; j < games[i].size(); j++) {
            std::vector<unsigned char> gameStateMapPred = GenerateMapPredInput(games[i][j]);
            singleGameSnapshots.push_back(gameStateMapPred);
            singleGameLocations.push_back(games[i][j].fugSquare);
        }
        gameMapSnapshots.push_back(singleGameSnapshots);
        fugitiveLocations.push_back(singleGameLocations);
    }

    return socket->TrainModel(gameMapSnapshots, fugitiveLocations, fugitivesWon, name);
}

std::vector<float> ModelDataLoader::GetTileProbabilities(std::vector<GameSnapshot> gameSnapshot, std::string name) const
{
    std::vector<std::vector<unsigned char>> gameSnapshotTensor;
    gameSnapshotTensor.reserve(gameSnapshot.size());
    for (auto& s : gameSnapshot) {
        gameSnapshotTensor.push_back(GenerateMapPredInput(s));
    }

    return socket->GetTileProbabilities(gameSnapshotTensor, name);
}

// GameMove ModelDataLoader::GetFugitiveMove(const GameSnapshot &snapshot) const
// {

// }

// std::vector<GameMove> ModelDataLoader::GetDetectiveMoves(const GameSnapshot &snapshot) const
// {

// }
