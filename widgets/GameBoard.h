#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QLabel>
#include <QMouseEvent>

#include "Shared.h"

class GameBoard : public QLabel
{
    Q_OBJECT
public:
    GameBoard(QWidget *parent = nullptr);
    bool ReadLocationFile(const char* filename);
    bool ReadTravelFile(const char* filename, TRAVEL_TYPE type);
    void UpdateDrawCheck(bool checked, DRAW_OBJ obj);

    std::vector<matrixVal> GetTaxiMatrix() const {return taxiMatrix;}
    std::vector<matrixVal> GetBusMatrix() const {return busMatrix;}
    std::vector<matrixVal> GetUndergroundMatrix() const {return undergroundMatrix;}
    std::vector<matrixVal> GetFerryMatrix() const {return ferryMatrix;}
signals:
    void BoardClicked(QPointF pos);

public slots:
    void GamestateUpdated(GameSnapshot snapshot);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<QPointF> points;

    // Drawing the connections to check correctness
    std::vector<matrixVal> taxiMatrix, busMatrix, undergroundMatrix, ferryMatrix;
    bool drawStation, drawTaxi, drawBus, drawUnderground, drawFerry;

    // Draw gamestate
    GameSnapshot snapshot;
};

#endif // GAMEBOARD_H
