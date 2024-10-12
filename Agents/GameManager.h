#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>

#include "DetectiveAi.h"
#include "FugitiveAi.h"
#include "MoveGenerator.h"
#include "../Game.h"

class GameManager : public QObject
{
    Q_OBJECT
public:
    GameManager(const MoveGenerator* mg, QObject* parent = nullptr);

public slots:
    void SimulateGame();
signals:
    void GamestateUpdated(GameSnapshot snapshot);

private:
    bool gameSimulating;
    DetectiveAi detective;
    FugitiveAi fugitive;
    const MoveGenerator* moveGenerator;
    Game game;
};

#endif // GAMEMANAGER_H
