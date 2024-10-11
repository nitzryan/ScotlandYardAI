#include "GameBoard.h"
#include <QPainter>
#include <fstream>

const int NUM_POINTS = 199;

GameBoard::GameBoard(QWidget *parent) : QLabel(parent) {
    points.reserve(NUM_POINTS);
}

bool GameBoard::ReadLocationFile(const char *filename)
{
    std::ifstream file;
    file.open("rules/locations.txt");
    if (!file.is_open()) {
        return false;
    }

    for (int i = 0; i < NUM_POINTS; i++) {
        float x, y;
        char comma;
        file >> x >> comma >> y;
        points.emplace_back(x,y);
    }

    file.close();
    return file.good();
}

void GameBoard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    QPointF mousePos = event->position();
    mousePos.setX(mousePos.x() / size().width());
    mousePos.setY(mousePos.y() / size().height());
    emit BoardClicked(mousePos);

    QLabel::mousePressEvent(event);
}

void GameBoard::paintEvent(QPaintEvent* event) {
    QLabel::paintEvent(event);

    QPainter painter(this);
    QPen pen(Qt::yellow);


    int w = size().width();
    int h = size().height();
    int r = w / 110;

    pen.setWidth(4);
    painter.setPen(pen);

    for (auto& p : points) {
        painter.drawEllipse(QPoint(p.x() * w, p.y() * h), r, r);
    }
    for (auto& taxi : taxis) {
        painter.drawLine(taxi);
    }

    pen.setColor(Qt::green);
    for (auto& bus : buses) {
        painter.drawLine(bus);
    }

    pen.setColor(Qt::red);
    for (auto& highway : highways) {
        painter.drawLine(highway);
    }


}
