#include "ScoreWidget.h"

#include <QPainter>

const int MIN_HEIGHT = 120;
const int BAR_HEIGHT = 40;
const int TEXT_PADDING = 10;

ScoreWidget::ScoreWidget(QWidget *parent)
    : QWidget{parent}
{
    detectiveChance = 0.6f;
    fugitiveChance = 0.4f;

    setMinimumHeight(MIN_HEIGHT);
}

void ScoreWidget::UpdateWinChance(float det, float fug)
{
    detectiveChance = det;
    fugitiveChance = fug;

    repaint();
}

void ScoreWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRectF totalRect = this->rect();
    QRectF detectiveRect = QRectF(totalRect.topLeft(), totalRect.topRight() + QPointF(0, BAR_HEIGHT));
    QRectF fugitiveRect = QRectF(totalRect.bottomLeft() + QPointF(0, -BAR_HEIGHT), totalRect.bottomRight());

    painter.fillRect(detectiveRect.left(), detectiveRect.top(), detectiveRect.width() * detectiveChance, detectiveRect.height(), Qt::blue);
    painter.fillRect(fugitiveRect.left(), fugitiveRect.top(), fugitiveRect.width() * fugitiveChance, fugitiveRect.height(), Qt::red);

    auto drawText = [](QRectF& rect, float prob, QPainter& painter) {
        QRectF textRect;
        QString numString = QString::number(prob, 'f', 2);
        if (prob > 0.5)
        {
            textRect = painter.boundingRect(rect, Qt::AlignLeft | Qt::AlignVCenter, numString);
            textRect.setLeft(textRect.left() + TEXT_PADDING);
            textRect.setRight(textRect.right() + 2 * TEXT_PADDING);
        } else
        {
            textRect = painter.boundingRect(rect, Qt::AlignRight | Qt::AlignVCenter, numString);
            textRect.setLeft(textRect.left() - 2 * TEXT_PADDING);
            textRect.setRight(textRect.right() - TEXT_PADDING);
        }

        QFont font = painter.font();
        font.setWeight(QFont::Bold);
        painter.setFont(font);
        painter.drawText(textRect, Qt::AlignCenter, numString);
    };

    drawText(detectiveRect, detectiveChance, painter);
    drawText(fugitiveRect, fugitiveChance, painter);
}
