#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QLabel>
#include <QMouseEvent>

enum class TRAVEL_TYPE {
    TAXI,
    BUS,
    UNDERGROUND,
    FERRY
};

enum class DRAW_OBJ {
    STATION,
    TAXI,
    BUS,
    UNDERGROUND,
    FERRY
};

class GameBoard : public QLabel
{
    Q_OBJECT
public:
    GameBoard(QWidget *parent = nullptr);
    bool ReadLocationFile(const char* filename);
    bool ReadTravelFile(const char* filename, TRAVEL_TYPE type);
    void UpdateDrawCheck(bool checked, DRAW_OBJ obj);
signals:
    void BoardClicked(QPointF pos);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<QPointF> points;
    struct matrixVal {
        unsigned char row;
        unsigned char col;
    };

    std::vector<matrixVal> taxiMatrix, busMatrix, undergroundMatrix, ferryMatrix;
    bool drawStation, drawTaxi, drawBus, drawUnderground, drawFerry;
};

#endif // GAMEBOARD_H
