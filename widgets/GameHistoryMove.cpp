#include "GameHistoryMove.h"
#include "ui_GameHistoryMove.h"

GameHistoryMove::GameHistoryMove(const GameSnapshot& snapshot, bool shouldShow, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameHistoryMove)
{
    ui->setupUi(this);

    fugitiveSnapshot = snapshot;
    ui->lbl_TurnId->setText(QString::number(snapshot.turn));
    QString moveText;
    if (snapshot.fugMove & (unsigned char)GameMovesType::TAXI) {
        moveText = "Taxi";
    } else if (snapshot.fugMove & (unsigned char)GameMovesType::BUS) {
        moveText = "Bus";
    } else if (snapshot.fugMove & (unsigned char)GameMovesType::UNDERGROUND) {
        moveText = "Underground";
    } else if (snapshot.fugMove & (unsigned char)GameMovesType::MYSTERY) {
        moveText = "Mystery";
    } else { // No Move Made
        moveText = "No Move";
    }
    ui->pb_XMove->setText(moveText);
    connect(ui->pb_XMove, &QPushButton::clicked, [this]() {
        emit SnapshotSelected(fugitiveSnapshot);
    });

    if (snapshot.fugMove & (unsigned char)GameMovesType::DOUBLE) {
        ui->pb_DetectiveMove->setText("Double");
        ui->pb_DetectiveMove->setEnabled(false);
        ui->pb_DetectiveMove->setStyleSheet("QPushButton {"
                                            "background-color: transparent;"
                                            "border:none;"
                                            "}");
    } else {
        ui->pb_DetectiveMove->setVisible(false);
    }

    ShowFugitiveLocation(shouldShow);
}

GameHistoryMove::~GameHistoryMove()
{
    delete ui;
}

void GameHistoryMove::AddDetectiveMove(const GameSnapshot &snapshot)
{
    detectiveSnapshot = snapshot;
    connect(ui->pb_DetectiveMove, &QPushButton::clicked, [this]() {
        emit SnapshotSelected(detectiveSnapshot);
    });
    ui->pb_DetectiveMove->setVisible(true);
}

void GameHistoryMove::ShowFugitiveLocation(bool shouldShow)
{
    shouldShow |= (fugitiveSnapshot.turn == REVEAL_TURN_1 ||
                   fugitiveSnapshot.turn == REVEAL_TURN_2 ||
                   fugitiveSnapshot.turn == REVEAL_TURN_3 ||
                   fugitiveSnapshot.turn == FINAL_TURN);

    if (shouldShow) {
        ui->lbl_XLocation->setText(QString::number(fugitiveSnapshot.fugSquare));
    } else {
        ui->lbl_XLocation->setText("???");
    }
}
