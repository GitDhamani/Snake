#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int scale;
    int cols;
    int rows;
    QTimer* MoveSnake = nullptr;
    QTimer* Restart;
    int headDirectX;
    int headDirectY;
    int headX;
    int headY;
    void SpawnRandomFood();
    int foodX;
    int foodY;
    QVector<QPoint> Body;
    int BodySize;
    int prevHeadX;
    int prevHeadY;
    void startGame();
    int maxScore;
    void updateScore();
    QLabel* label;
    QPixmap* pixmap;
    QPixmap* pixmapOrig;
    void drawOnPixmap();
    QPainter* painter;
    int intspeed;

protected:
    void keyPressEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
