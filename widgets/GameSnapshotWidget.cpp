#include "GameSnapshotWidget.h"
#include "ui_GameSnapshotWidget.h"

GameSnapshotWidget::GameSnapshotWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameSnapshotWidget)
{
    ui->setupUi(this);

    ui->detectiveSnapshot1->SetId(1);
    ui->detectiveSnapshot2->SetId(2);
    ui->detectiveSnapshot3->SetId(3);
    ui->detectiveSnapshot4->SetId(4);
    ui->detectiveSnapshot5->SetId(5);
}

GameSnapshotWidget::~GameSnapshotWidget()
{
    delete ui;
}

void GameSnapshotWidget::UpdateSnapshot(const GameSnapshot &snapshot)
{
    ui->detectiveSnapshot1->UpdateInfo(snapshot.detSquare[0], snapshot.detTaxis[0], snapshot.detBuses[0], snapshot.detUndergrounds[0]);
    ui->detectiveSnapshot2->UpdateInfo(snapshot.detSquare[1], snapshot.detTaxis[1], snapshot.detBuses[1], snapshot.detUndergrounds[1]);
    ui->detectiveSnapshot3->UpdateInfo(snapshot.detSquare[2], snapshot.detTaxis[2], snapshot.detBuses[2], snapshot.detUndergrounds[2]);
    ui->detectiveSnapshot4->UpdateInfo(snapshot.detSquare[3], snapshot.detTaxis[3], snapshot.detBuses[3], snapshot.detUndergrounds[3]);
    ui->detectiveSnapshot5->UpdateInfo(snapshot.detSquare[4], snapshot.detTaxis[4], snapshot.detBuses[4], snapshot.detUndergrounds[4]);
    ui->fugitiveSnapshot->UpdateInfo(snapshot);

    ui->lbl_TurnCount->setText(QString::number(snapshot.turn));
    if (snapshot.isFugitiveMove) {
        ui->lbl_CurrentTurnSide->setAlignment(Qt::AlignLeft);
    } else {
        ui->lbl_CurrentTurnSide->setAlignment(Qt::AlignRight);
    }
    ui->lbl_CurrentTurnSide->update();
}

void GameSnapshotWidget::SetShowX(bool shouldShow)
{
    ui->fugitiveSnapshot->SetHideTile(!shouldShow);
}
