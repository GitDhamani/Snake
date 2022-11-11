#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPen>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Snake Game");
    setFixedSize(500,500);
    statusBar()->hide();
    menuBar()->hide();

    label = new QLabel(this);
    pixmap = new QPixmap(":/background1.jpg");
    pixmapOrig = new QPixmap(":/background1.jpg");
    label->setPixmap(*pixmap);
    label->setScaledContents(true);
    label->setAutoFillBackground(true);
    label->resize(500,500);
    painter = new QPainter(pixmap);

    scale = 100;
    rows = pixmap->height()/scale;
    cols = pixmap->width()/scale;
    maxScore = 0;

    startGame();

    Restart = new QTimer(this);
    Restart->setInterval(1000);
    Restart->setSingleShot(true); 

    //Restart The Game
    connect(Restart, &QTimer::timeout, [=]{
        startGame();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SpawnRandomFood()
{
    while(true) //Don't Spawn Food in the Head or Body of the Snake
    {
    foodX = QRandomGenerator::global()->bounded(0, cols);
    foodY = QRandomGenerator::global()->bounded(0, rows);

    bool foodinside = false;
    for (int i = 0; i<BodySize; i++)
        if (Body[i].x() == foodX && Body[i].y() == foodY)
            foodinside = true;
    if (foodX == headX && foodY == headY)
        foodinside = true;

    if (foodinside == false)
        break;
    }
}

void MainWindow::startGame()
{
    BodySize = 0;
    Body.clear();

    //Initial Position of Snake Head
    headX = 10;
    headY = 2;

    //Initially Start with the Head Travelling Vertically Downward
    headDirectX = 0;
    headDirectY = 1;

    SpawnRandomFood();

    if (MoveSnake != nullptr)
        delete MoveSnake;

    MoveSnake = new QTimer(this);
    intspeed = 150;
    MoveSnake->setInterval(intspeed);
    MoveSnake->start();

    //Call the Repaint event
    connect(MoveSnake, &QTimer::timeout, [this]{
        painter->drawPixmap(0, 0, 2000,2000, *pixmapOrig, 0, 0, 2000,2000);
        drawOnPixmap();
        label->setPixmap(*pixmap);
    });
}

void MainWindow::updateScore()
{
    if (BodySize > maxScore)
    {
        maxScore = BodySize;
        setWindowTitle("Snake Game. Max Score: " + QString::number(maxScore));
    }
}

void MainWindow::drawOnPixmap()
{
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::black);  //can also use .setColor(QColor("#XXXXXX"));
    painter->setPen(pen);

    //Draw Body
    if (BodySize > 0)
    {
    for (int i = 0; i < BodySize-1; i++)
        Body[i] = Body[i+1];
    Body[BodySize-1] = QPoint(headX, headY);
    painter->setBrush(Qt::green);
    for (int i = 0; i < BodySize; i++)
    painter->drawEllipse(Body[i].x()*scale, Body[i].y()*scale, scale, scale);
    }

    //Move Head of Snake
    headX = headX + headDirectX;
    headY = headY + headDirectY;

    //When Snake Hits Bounds of Window with no body, it is constrained.
    if (headX < 0) {headX = 0; headDirectX = 0; headDirectY = 0;}
    if (headX > cols - 1) {headX = cols-1; headDirectX = 0; headDirectY = 0;}
    if (headY < 0) {headY = 0; headDirectX = 0; headDirectY = 0;}
    if (headY > rows - 1) {headY = rows-1; headDirectX = 0; headDirectY = 0;}

    //When Snake Hits Bounds of Window and Body > 0, Game Over
    if ((headX < 0 || headX > cols-1 || headY < 0 ||  headY > rows-1) && BodySize>0)
    {
        MoveSnake->stop();
        updateScore();
        Restart->start();
    }

    //Draw Head
    painter->setBrush(Qt::red);
    painter->drawEllipse(headX*scale, headY*scale, scale, scale);

    //Draw Food
    painter->setBrush(QColor("#ffaaff"));
    painter->drawEllipse(foodX*scale, foodY*scale, scale, scale);

    //Detect Head Collision With Food
    if (headX == foodX && headY == foodY)
    {
        qDebug() << "Head Collides with Food";
        SpawnRandomFood();
        BodySize++;
        Body << QPoint(prevHeadX, prevHeadY);
        if (intspeed>100)
        intspeed-=5;
        MoveSnake->setInterval(intspeed);
    }

    //Detect Head Collision with Body
    for (int i = 0; i<BodySize; i++)
        if (Body[i].x() == headX && Body[i].y() == headY)
        {
            MoveSnake->stop();
            updateScore();
            Restart->start();
        }

    prevHeadX = headX;
    prevHeadY = headY;

}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {

    case Qt::Key_Up:
        qDebug() << "Up Arrow";
        if (headDirectY == 1) return; //Prevent Doubling Back
        headDirectX = 0;
        headDirectY = -1;
    break;

    case Qt::Key_Down:
        qDebug() << "Down Arrow";
        if (headDirectY == -1) return; //Prevent Doubling Back
        headDirectX = 0;
        headDirectY = 1;
    break;

    case Qt::Key_Right:
        qDebug() << "Right Arrow";
        if (headDirectX == -1) return; //Prevent Doubling Back
        headDirectX = 1;
        headDirectY = 0;
    break;

    case Qt::Key_Left:
        qDebug() << "Left Arrow";
        if (headDirectX == 1) return; //Prevent Doubling Back
        headDirectX = -1;
        headDirectY = 0;
    break;

    default:
        QMainWindow::keyPressEvent(event);
    }
}

