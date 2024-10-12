#include "DetectiveSnapshot.h"
#include "ui_DetectiveSnapshot.h"

DetectiveSnapshot::DetectiveSnapshot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DetectiveSnapshot)
{
    ui->setupUi(this);
    id = 0;

    UpdateInfo(0,0,0,0);
}

DetectiveSnapshot::~DetectiveSnapshot()
{
    delete ui;
}

void DetectiveSnapshot::SetId(unsigned char id)
{
    this->id = id;
    ui->lbl_DetectiveId->setText(QString("Detective ").append(QString::number(id)));
}

void DetectiveSnapshot::UpdateInfo(unsigned char tile, unsigned char taxis, unsigned char buses, unsigned char undergrounds)
{
    ui->lbl_TileId->setText(QString::number(tile));
    ui->lbl_TaxiNum->setText(QString::number(taxis));
    ui->lbl_BusNum->setText(QString::number(buses));
    ui->lbl_UndergroundNum->setText(QString::number(undergrounds));
}
