#include "GameManager.h"
#include <chrono>
#include <thread>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>

GameManager::GameManager(const MoveGenerator* mg, QObject* parent) : QObject(parent) {
    moveGenerator = mg;
    gameSimulating = false;
}

void GameManager::TrainModel(std::string modelName, int generations, int gamesPerGeneration)
{
    for (int i = 0; i < generations; i++) {
        std::vector<std::vector<GameSnapshot>> games;
        std::vector<bool> fugitivesWon;
        games.reserve(gamesPerGeneration);
        fugitivesWon.reserve(gamesPerGeneration);
        for (int j = 0; j < gamesPerGeneration; j++) {
            while (gameSimulating) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            SimulateGame(false, 0);
            GameResult gameResult = gameFuture.result();
            games.push_back(gameResult.snapshots);
            fugitivesWon.push_back(gameResult.fugitiveWon);
        }

        float loss = dataLoader.Train(games, fugitivesWon, modelName + "_g" + std::to_string(i));
        qDebug() << "Training Loss for Generation " << i + 1 << ": " << loss;
        emit TrainingPercentUpdate((float)(i + 1) / generations);
    }

    emit TrainingComplete();
}

void GameManager::SnapshotSelected(GameSnapshot snapshot)
{
    std::vector<GameSnapshot> snapshots = game.GetGameStates();
    std::vector<GameSnapshot> snapshotsUntilSelected;
    for (auto& i : snapshots) {
        if (i == snapshot) {
            snapshotsUntilSelected.push_back(i);
            break;
        }
        snapshotsUntilSelected.push_back(i);
    }

    std::vector<float> probabilities = dataLoader.GetTileProbabilities(snapshotsUntilSelected, "default_g0");
    emit FugitiveProbabilitiesUpdated(probabilities);
    // qDebug() << "Tile Probabilities";
    // for (int i = 0; i < probabilities.size(); i++) {
    //     qDebug() << "Tile " << i + 1 << " : " << probabilities[i];
    // }
}

void GameManager::SimulateGame(bool shouldEmit, int thinkMs)
{
    if (gameSimulating) {
        return;
    }

    gameSimulating = true;

    if (shouldEmit) {
        emit StartGame();
    }

    gameFuture = QtConcurrent::run([this, shouldEmit, thinkMs]() {
        game = Game();
        while (!game.DetectivesWon() && !game.FugitiveWon()) {
            GameSnapshot snapshot = game.GetCurrentState();
            if (snapshot.isFugitiveMove) {
                game.MakeFugitiveMove(fugitive.GetMove(moveGenerator, snapshot));
            } else {
                game.MakeDetectiveMoves(detective.GetMoves(moveGenerator, snapshot));
            }

            if (shouldEmit) {
                emit GamestateUpdated(game.GetCurrentState());
                emit ScoreUpdated(game.GetDetectiveScore(), game.GetFugitiveScore());
            }

            if (thinkMs > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(thinkMs));
            }
        }

        gameSimulating = false;
        GameResult result;
        result.fugitiveWon = game.FugitiveWon();
        result.snapshots = game.GetGameStates();
        return result;
    });

}
