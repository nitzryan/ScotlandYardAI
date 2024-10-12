#ifndef DETECTIVESNAPSHOT_H
#define DETECTIVESNAPSHOT_H

#include <QWidget>

namespace Ui {
class DetectiveSnapshot;
}

class DetectiveSnapshot : public QWidget
{
    Q_OBJECT

public:
    explicit DetectiveSnapshot(QWidget *parent = nullptr);
    ~DetectiveSnapshot();
    void SetId(unsigned char id);
    void UpdateInfo(unsigned char tile, unsigned char taxis, unsigned char buses, unsigned char undergrounds);

private:
    Ui::DetectiveSnapshot *ui;
    unsigned char id, tile, numTaxis, numBuses, numUndergrounds;
};

#endif // DETECTIVESNAPSHOT_H
