#ifndef GAMEHISTORY_H
#define GAMEHISTORY_H

#include <QWidget>
#include "Shared.h"
#include <vector>

class GameHistoryMove;

namespace Ui {
class GameHistory;
}

class GameHistory : public QWidget
{
    Q_OBJECT

public:
    explicit GameHistory(QWidget *parent = nullptr);
    ~GameHistory();
public slots:
    void AddSnapshot(const GameSnapshot& snapshot);
    void ShowFugitiveLocation(bool shouldShow);
    void ClearGame();
signals:
    void SnapshotSelected(const GameSnapshot& snapshot);

private:
    Ui::GameHistory *ui;
    std::vector<GameHistoryMove*> moves;
    bool shouldShow;
};

#endif // GAMEHISTORY_H
