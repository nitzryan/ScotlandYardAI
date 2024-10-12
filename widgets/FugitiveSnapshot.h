#ifndef FUGITIVESNAPSHOT_H
#define FUGITIVESNAPSHOT_H

#include <QWidget>
#include "Shared.h"

namespace Ui {
class FugitiveSnapshot;
}

class FugitiveSnapshot : public QWidget
{
    Q_OBJECT

public:
    explicit FugitiveSnapshot(QWidget *parent = nullptr);
    ~FugitiveSnapshot();
    void UpdateInfo(const GameSnapshot& snapshot);
    void SetHideTile(bool shouldHide);
private:
    Ui::FugitiveSnapshot *ui;
    GameSnapshot snapshot;
    bool hideTile;
};

#endif // FUGITIVESNAPSHOT_H
