#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

#include <QThread>
#include <QtConcurrent/QtConcurrent>

#include "Shared.h"

const int STATE_GENERATING_MAP = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, &MainWindow::UpdateHeaderRequest, this, &MainWindow::UpdateHeaderText);
    connect(ui->pb_GenerateMap, &QPushButton::clicked, this, &MainWindow::GenerateMap);
    currentState = 0;

    connect(ui->board, &GameBoard::BoardClicked, this, [this](QPointF mousePos){
        mapPoint = mousePos;
        mapClicked = true;
    });

    connect(ui->pb_ReloadMap, &QPushButton::clicked, this, [this](){
        ui->board->ReadLocationFile("rules/locations.txt");
        ui->board->ReadTravelFile("rules/taxis.txt", TRAVEL_TYPE::TAXI);
        ui->board->ReadTravelFile("rules/buses.txt", TRAVEL_TYPE::BUS);
        ui->board->ReadTravelFile("rules/underground.txt", TRAVEL_TYPE::UNDERGROUND);
        ui->board->ReadTravelFile("rules/ferrys.txt", TRAVEL_TYPE::FERRY);
        ui->board->repaint();
    });
    ui->pb_ReloadMap->click();

    moveGenerator = new MoveGenerator(ui->board->GetTaxiMatrix(),
                                ui->board->GetBusMatrix(),
                                ui->board->GetUndergroundMatrix(),
                                ui->board->GetFerryMatrix());

    gameManager = new GameManager(moveGenerator);

    connect(ui->cb_Stations, &QCheckBox::clicked, [this](bool checked) {
        ui->board->UpdateDrawCheck(checked, DRAW_OBJ::STATION);
    });
    connect(ui->cb_Taxis, &QCheckBox::clicked, [this](bool checked) {
        ui->board->UpdateDrawCheck(checked, DRAW_OBJ::TAXI);
    });
    connect(ui->cb_Buses, &QCheckBox::clicked, [this](bool checked) {
        ui->board->UpdateDrawCheck(checked, DRAW_OBJ::BUS);
    });
    connect(ui->cb_Highways, &QCheckBox::clicked, [this](bool checked) {
        ui->board->UpdateDrawCheck(checked, DRAW_OBJ::UNDERGROUND);
    });
    connect(ui->cb_Ferry, &QCheckBox::clicked, [this](bool checked) {
        ui->board->UpdateDrawCheck(checked, DRAW_OBJ::FERRY);
    });

    connect(gameManager, &GameManager::GamestateUpdated, ui->board, &GameBoard::GamestateUpdated);
    connect(ui->pb_StartGame, &QPushButton::clicked, gameManager, &GameManager::SimulateGame);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete moveGenerator;
    delete gameManager;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        escClicked = true;
    }
}

void MainWindow::UpdateHeaderText(QString text)
{
    ui->lbl_Header->setText(text);
}

void MainWindow::GenerateMap() {
    if (currentState != 0) {
        return;
    }

    currentState = STATE_GENERATING_MAP;

    escClicked = false;
    mapFuture = QtConcurrent::run([this]() {
        std::vector<QPointF> points;
        points.reserve(NUM_POINTS);
        for (int i = 1; i <= NUM_POINTS; i++) {
            emit UpdateHeaderRequest(QString("Select Point ").append(QString::number(i)).append("  (Esc to cancel)"));
            mapClicked = false;
            while (!mapClicked && !escClicked) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            if (escClicked) {
                break;
            } else {
                points.push_back(mapPoint);
            }
        }

        currentState = 0;
        if (points.size() != NUM_POINTS) {
            emit UpdateHeaderRequest("Map Generation Exited");
            return;
        }

        std::ofstream file;
        file.open("rules/locations.txt");
        if (!file.is_open()) {
            emit UpdateHeaderRequest("Failed to map file");
            return;
        }

        for (auto& i : points) {
            file << i.x() << "," << i.y() << "\n";
        }

        if (file.good()) {
            emit UpdateHeaderRequest("Map Generation Suceeded");
        } else {
            emit UpdateHeaderRequest("Map Generation Write Error");
        }
        file.close();
        if (file.fail()) {
            emit UpdateHeaderRequest("Map Generation File Closing Error");
        }
    });


}
