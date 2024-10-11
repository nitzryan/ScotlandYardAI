#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFuture>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void UpdateHeaderText(QString text);
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void UpdateHeaderRequest(QString text);

private:
    Ui::MainWindow *ui;
    int currentState;

    // Handles state for Generating Map
    void GenerateMap();
    QPointF mapPoint;
    bool mapClicked;
    bool escClicked;
    QFuture<void> mapFuture;
};
#endif // MAINWINDOW_H
