#include "FugitiveSnapshot.h"
#include "ui_FugitiveSnapshot.h"

FugitiveSnapshot::FugitiveSnapshot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FugitiveSnapshot)
{
    ui->setupUi(this);
    snapshot = GameSnapshot();
    snapshot.turn = 0;
    snapshot.fugSquare = 0;
    snapshot.fugTaxis = 0;
    snapshot.fugBuses = 0;
    snapshot.fugUndergrounds = 0;
    snapshot.fugDoubles = 0;
    snapshot.fugMysteries = 0;

    UpdateInfo(snapshot);
}

FugitiveSnapshot::~FugitiveSnapshot()
{
    delete ui;
}

void FugitiveSnapshot::UpdateInfo(const GameSnapshot &snapshot)
{
    this->snapshot = snapshot;

    if (hideTile
        && snapshot.turn != REVEAL_TURN_1
        && snapshot.turn != REVEAL_TURN_2
        && snapshot.turn != REVEAL_TURN_3
        && snapshot.turn != FINAL_TURN)
    {
        ui->lbl_TileId->setText("???");
    } else {
        ui->lbl_TileId->setText(QString::number(snapshot.fugSquare));
    }

    ui->lbl_NumTaxis->setText(QString::number(snapshot.fugTaxis));
    ui->lbl_NumBuses->setText(QString::number(snapshot.fugBuses));
    ui->lbl_NumUndergrounds->setText(QString::number(snapshot.fugUndergrounds));
    ui->lbl_NumMysteries->setText(QString::number(snapshot.fugMysteries));
    ui->lbl_NumDoubles->setText(QString::number(snapshot.fugDoubles));
}

void FugitiveSnapshot::SetHideTile(bool shouldHide)
{
    hideTile = shouldHide;
    UpdateInfo(snapshot);
}
