#include "GameManager.h"
#include <chrono>
#include <thread>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>

GameManager::GameManager(const MoveGenerator* mg, QObject* parent) : QObject(parent) {
    moveGenerator = mg;
    gameSimulating = false;
}

void GameManager::SimulateGame()
{
    if (gameSimulating) {
        return;
    }

    gameSimulating = true;
    emit StartGame();

    gameFuture = QtConcurrent::run([this]() {
        game = Game();
        while (!game.DetectivesWon() && !game.FugitiveWon()) {
            GameSnapshot snapshot = game.GetCurrentState();
            if (snapshot.isFugitiveMove) {
                game.MakeFugitiveMove(fugitive.GetMove(moveGenerator, snapshot));
            } else {
                game.MakeDetectiveMoves(detective.GetMoves(moveGenerator, snapshot));
            }

            emit GamestateUpdated(game.GetCurrentState());
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        gameSimulating = false;
        qDebug() << "Game Is Over";
        if (game.DetectivesWon())
            qDebug() << "Detectives Won";
        if (game.FugitiveWon())
            qDebug() << "Fugitives Won";
    });

}
