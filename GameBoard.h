#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QLabel>
#include <QMouseEvent>

class GameBoard : public QLabel
{
    Q_OBJECT
public:
    GameBoard(QWidget *parent = nullptr);
    bool ReadLocationFile(const char* filename);
signals:
    void BoardClicked(QPointF pos);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<QPointF> points;
    std::vector<QLineF> taxis, buses, highways;
};

#endif // GAMEBOARD_H
