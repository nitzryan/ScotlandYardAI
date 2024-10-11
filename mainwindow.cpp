#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

#include <QThread>
#include <QtConcurrent/QtConcurrent>

const int NUM_POINTS = 199;
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

    ui->board->ReadLocationFile("rules/locations.txt");
}

MainWindow::~MainWindow()
{
    delete ui;
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
            if (i == 128) { // Version of board used has no 128th square
                points.push_back({1.1, 1.1});
                continue;
            }
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
