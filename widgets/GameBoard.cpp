#include "GameBoard.h"
#include <QPainter>
#include <fstream>

const char INVALID_SNAPSHOT_TURN = 100;

GameBoard::GameBoard(QWidget *parent) : QLabel(parent) {
    points.reserve(NUM_POINTS);
    drawStation = false;
    drawTaxi = false;
    drawBus = false;
    drawUnderground = false;
    drawFerry = false;

    snapshot.turn = INVALID_SNAPSHOT_TURN;
}

bool GameBoard::ReadLocationFile(const char *filename)
{
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        return false;
    }

    points.clear();
    for (int i = 0; i < NUM_POINTS; i++) {
        float x, y;
        char comma;
        file >> x >> comma >> y;
        points.emplace_back(x,y);
    }

    file.close();
    return file.good();
}

bool GameBoard::ReadTravelFile(const char* filename, TRAVEL_TYPE type) {
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        return false;
    }

    std::vector<matrixVal>* matrix;
    switch (type) {
    case TRAVEL_TYPE::TAXI:
        matrix = &taxiMatrix;
        break;
    case TRAVEL_TYPE::BUS:
        matrix = &busMatrix;
        break;
    case TRAVEL_TYPE::UNDERGROUND:
        matrix = &undergroundMatrix;
        break;
    case TRAVEL_TYPE::FERRY:
        matrix = &ferryMatrix;
        break;
    }

    int num_edges;
    file >> num_edges;
    matrix->clear();
    matrix->reserve(2 * num_edges);
    for (int i = 0; i < num_edges; i++) {
        int r, c;
        char comma;
        file >> r >> comma >> c;
        if (c < r) {
            qDebug() << "Error: Column < Row for " << r << "," << c;
            continue;
        }
        matrix->push_back({(unsigned char)r,(unsigned char)c});
        matrix->push_back({(unsigned char)c,(unsigned char)r});
    }

    file.close();
    return file.good();
}

void GameBoard::UpdateDrawCheck(bool checked, DRAW_OBJ obj)
{
    switch(obj) {
    case DRAW_OBJ::STATION:
        drawStation = checked;
        break;
    case DRAW_OBJ::TAXI:
        drawTaxi = checked;
        break;
    case DRAW_OBJ::BUS:
        drawBus = checked;
        break;
    case DRAW_OBJ::UNDERGROUND:
        drawUnderground = checked;
        break;
    case DRAW_OBJ::FERRY:
        drawFerry = checked;
        break;
    }
    repaint();
}

void GameBoard::GamestateUpdated(GameSnapshot s)
{
    snapshot = s;
    probabilities = {};
    repaint();
}

void GameBoard::ProbabilityMapUpdated(std::vector<float> probs)
{
    probabilities = probs;
    repaint();
}

void GameBoard::ShouldDrawTokens(bool shouldDraw)
{
    drawTokens = shouldDraw;
    repaint();
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
    const QRect& rect = event->rect();

    QLabel::paintEvent(event);

    QPainter painter(this);
    QPen pen(Qt::black);


    int w = size().width();
    int h = size().height();
    int r = w / 110;

    pen.setWidth(4);
    painter.setPen(pen);

    if (drawStation) {
        for (auto& p : points) {
            painter.drawEllipse(QPoint(p.x() * w, p.y() * h), r, r);
        }
    }

    if (drawTaxi) {
        pen.setColor(Qt::yellow);
        painter.setPen(pen);
        for (auto& taxi : taxiMatrix) {
            QPointF p1 = points.at(taxi.row - 1);
            QPointF p2 = points.at(taxi.col - 1);
            p1.setX(p1.x() * w);
            p1.setY(p1.y() * h);
            p2.setX(p2.x() * w);
            p2.setY(p2.y() * h);
            painter.drawLine(p1, p2);
        }
    }

    if (drawBus) {
        pen.setColor(Qt::green);
        painter.setPen(pen);
        for (auto& bus : busMatrix) {
            QPointF p1 = points.at(bus.row - 1);
            QPointF p2 = points.at(bus.col - 1);
            p1.setX(p1.x() * w);
            p1.setY(p1.y() * h);
            p2.setX(p2.x() * w);
            p2.setY(p2.y() * h);
            painter.drawLine(p1, p2);
        }
    }


    if (drawUnderground) {
        pen.setColor(Qt::red);
        painter.setPen(pen);
        for (auto& highway : undergroundMatrix) {
            QPointF p1 = points.at(highway.row - 1);
            QPointF p2 = points.at(highway.col - 1);
            p1.setX(p1.x() * w);
            p1.setY(p1.y() * h);
            p2.setX(p2.x() * w);
            p2.setY(p2.y() * h);
            painter.drawLine(p1, p2);
        }
    }


    if (drawFerry) {
        pen.setColor(Qt::black);
        painter.setPen(pen);
        for (auto& ferry : ferryMatrix) {
            QPointF p1 = points.at(ferry.row - 1);
            QPointF p2 = points.at(ferry.col - 1);
            p1.setX(p1.x() * w);
            p1.setY(p1.y() * h);
            p2.setX(p2.x() * w);
            p2.setY(p2.y() * h);
            painter.drawLine(p1, p2);
        }
    }

    // Draw Gamestate
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    if (drawTokens) {
        if (snapshot.turn != INVALID_SNAPSHOT_TURN) {
            pen.setColor(Qt::white);
            brush.setColor(Qt::white);
            painter.setPen(pen);
            painter.setBrush(brush);

            QPointF& p = points[snapshot.fugSquare - 1];
            painter.drawEllipse(QPoint(p.x() * w, p.y() * h), r, r);
        }

        r *= 0.9;
        if (snapshot.turn != INVALID_SNAPSHOT_TURN) {
            pen.setColor(QColor(128,255,0));
            brush.setColor(QColor(128,255,0));
            painter.setPen(pen);
            painter.setBrush(brush);

            for (size_t i = 0; i < NUM_DETECTIVES; i++) {
                QPointF& p = points[snapshot.detSquare[i] - 1];
                painter.drawEllipse(QPoint(p.x() * w, p.y() * h), r, r);
            }
        }
    }

    // Draw probabilities for where model thinks Mr X is
    pen.setColor(Qt::blue);
    brush.setColor(Qt::blue);
    painter.setPen(pen);
    painter.setBrush(brush);
    for (size_t i = 0; i < probabilities.size(); i++) {
        int r_prob = r * sqrt(probabilities[i]) / 10;
        if (r_prob > 0) {
            QPointF& p = points[i];
            painter.drawEllipse(QPoint(p.x() * w, p.y() * h), r_prob, r_prob);
        }
    }
}
