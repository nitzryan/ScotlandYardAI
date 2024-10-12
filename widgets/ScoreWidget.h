#ifndef SCOREWIDGET_H
#define SCOREWIDGET_H

#include <QObject>
#include <QWidget>

class ScoreWidget : public QWidget
{
    Q_OBJECT
public:
    ScoreWidget(QWidget *parent = nullptr);

public slots:
    void UpdateWinChance(float detectiveChance, float fugutiveChance);

protected:
    void paintEvent(QPaintEvent* event);

private:
    float detectiveChance, fugitiveChance; // Both are chance of fugitive winning, but with different info
};

#endif // SCOREWIDGET_H
