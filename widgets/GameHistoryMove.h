#ifndef GAMEHISTORYMOVE_H
#define GAMEHISTORYMOVE_H

#include <QWidget>
#include "Shared.h"

namespace Ui {
class GameHistoryMove;
}

class GameHistoryMove : public QWidget
{
    Q_OBJECT

public:
    explicit GameHistoryMove(const GameSnapshot& snapshot, bool shouldShow, QWidget *parent = nullptr);
    ~GameHistoryMove();
    void AddDetectiveMove(const GameSnapshot& snapshot);
    void ShowFugitiveLocation(bool shouldShow);

signals:
    void SnapshotSelected(const GameSnapshot& snapshot);

private:
    Ui::GameHistoryMove *ui;
    GameSnapshot fugitiveSnapshot, detectiveSnapshot;
};

#endif // GAMEHISTORYMOVE_H
