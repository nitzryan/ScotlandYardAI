#include "GameHistory.h"
#include "ui_GameHistory.h"

#include "GameHistoryMove.h"

GameHistory::GameHistory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameHistory)
{
    ui->setupUi(this);
    shouldShow = false;
}

GameHistory::~GameHistory()
{
    delete ui;
}

void GameHistory::AddSnapshot(const GameSnapshot &snapshot)
{
    if (snapshot.wasFugitiveMove) {
        GameHistoryMove* move = new GameHistoryMove(snapshot, shouldShow, this);
        moves.push_back(move);
        connect(move, &GameHistoryMove::SnapshotSelected, this, &GameHistory::SnapshotSelected);
        ui->moveList->addWidget((QWidget*)move);
    } else {
        moves.back()->AddDetectiveMove(snapshot);
    }
}

void GameHistory::ShowFugitiveLocation(bool ss)
{
    shouldShow = ss;
    for (auto& move : moves) {
        move->ShowFugitiveLocation(shouldShow);
    }
}

void GameHistory::ClearGame()
{
    while (QLayoutItem* item = ui->moveList->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    moves.clear();
}
