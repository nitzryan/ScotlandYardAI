#include "TrainingWidget.h"
#include "ui_TrainingWidget.h"

TrainingWidget::TrainingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TrainingWidget)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        int numGenerations = ui->sbGenerations->value();
        int gamesPerGeneration = ui->sb_GamesPerGeneration->value();
        std::string modelName = ui->le_name->displayText().toStdString();
        if (modelName.length() == 0) {
            modelName = "default";
        }
        ui->pushButton->setVisible(false);
        ui->progressBar->setVisible(true);
        ui->sbGenerations->setEnabled(false);
        ui->sb_GamesPerGeneration->setEnabled(false);
        ui->le_name->setEnabled(false);
        emit TrainingStarted(modelName, numGenerations, gamesPerGeneration);
    });

    ui->progressBar->setVisible(false);
}

TrainingWidget::~TrainingWidget()
{
    delete ui;
}

void TrainingWidget::TrainingPercentUpdate(float perc)
{
    ui->progressBar->setValue((int)perc * 100);
}

void TrainingWidget::TrainingComplete()
{
    ui->pushButton->setVisible(true);
    ui->progressBar->setVisible(false);
    ui->sbGenerations->setEnabled(true);
    ui->sb_GamesPerGeneration->setEnabled(true);
    ui->le_name->setEnabled(true);
}
