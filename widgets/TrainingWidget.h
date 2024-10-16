#ifndef TRAININGWIDGET_H
#define TRAININGWIDGET_H

#include <QWidget>

namespace Ui {
class TrainingWidget;
}

class TrainingWidget : public QWidget
{
    Q_OBJECT

public:
    TrainingWidget(QWidget *parent = nullptr);
    ~TrainingWidget();

public slots:
    void TrainingPercentUpdate(float perc);
    void TrainingComplete();

signals:
    void TrainingStarted(std::string modelName, int numGenerations, int gamesPerGeneration);

private:
    Ui::TrainingWidget *ui;
};

#endif // TRAININGWIDGET_H
