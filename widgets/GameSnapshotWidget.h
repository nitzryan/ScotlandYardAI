#ifndef GAMESNAPSHOTWIDGET_H
#define GAMESNAPSHOTWIDGET_H

#include <QWidget>
#include "Shared.h"

namespace Ui {
class GameSnapshotWidget;
}

class GameSnapshotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameSnapshotWidget(QWidget *parent = nullptr);
    ~GameSnapshotWidget();

public slots:
    void UpdateSnapshot(const GameSnapshot& snapshot);
    void SetShowX(bool shouldShow);

private:
    Ui::GameSnapshotWidget *ui;
};

#endif // GAMESNAPSHOTWIDGET_H
