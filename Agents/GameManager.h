#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QFuture>

#include "DetectiveAi.h"
#include "FugitiveAi.h"
#include "MoveGenerator.h"
#include "../Game.h"
#include "nn/ModelDataLoader.h"

class GameManager : public QObject
{
    Q_OBJECT
public:
    GameManager(const MoveGenerator* mg, QObject* parent = nullptr);
public slots:
    void SimulateGame(bool shouldEmit, int thinkMs);
    void TrainModel(std::string modelName, int generations, int gamesPerGeneration);

    void SnapshotSelected(GameSnapshot snapshot);
signals:
    void GamestateUpdated(GameSnapshot snapshot);
    void ScoreUpdated(float detectiveScore, float fugitiveScore);
    void StartGame();

    void TrainingPercentUpdate(float perc);
    void TrainingComplete();

private:
    bool gameSimulating;
    DetectiveAi detective;
    FugitiveAi fugitive;
    const MoveGenerator* moveGenerator;
    Game game;

    struct GameResult {
        std::vector<GameSnapshot> snapshots;
        bool fugitiveWon;
    };

    QFuture<GameResult> gameFuture;

    ModelDataLoader dataLoader;
};

#endif // GAMEMANAGER_H
