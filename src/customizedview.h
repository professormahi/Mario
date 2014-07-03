#ifndef CUSTOMIZEDVIEW_H
#define CUSTOMIZEDVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QDebug>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsSimpleTextItem>
#include <QGradient>
#include <QString>
#include <QFile>
#include <iostream>
#include <fstream>
#include <string>
#include <QList>
#include <QMessageBox>
#include <QMap>
using namespace  std;

enum CollisonWithPipe
{
    FromUp,
    FromDown,
    FromUpEnd,
    None
};

class CustomizedView : public QGraphicsView
{
    Q_OBJECT
public:
    int HealthCount;
    int Score;
    int HealthInterval;
    QGraphicsPixmapItem *Mario;
    QGraphicsPixmapItem *Castle;
    QGraphicsScene *Scene;
    QGraphicsSimpleTextItem *EndOfGame;    

    QList<QGraphicsPixmapItem *> Pipes;
    QList<QGraphicsPixmapItem *> Coins;
    QList<QGraphicsPixmapItem *> Enemies;
    QList<QGraphicsPixmapItem *> Blocks;
    QMap<QGraphicsPixmapItem *, int> CoinBoxes;
    QGraphicsPixmapItem *CurrentCoin;
    QList<bool> EnemiesFlags;
    bool IsMarioUp;

    int t;
    explicit CustomizedView(QWidget *parent = 0);
    bool IsGameFinished;
    bool IsTimerRunning;
private:
    int ComputeY(int t);
    void MakeMap(QString MapAddress);
    bool PipeCollisions();
    bool EnemyCollesions();
    bool CoinBoxCollesions();
    bool EnemyCollesionsFromUp();
    bool BlockCollesions();
    
signals:
    
public slots:
    void keyPressEvent(QKeyEvent *event);
    void TimerShot();
    void EndTimer();
    void MovingEnemies();
    void CoinCollesions();
    void CoinCollectingFromBoxes();
    void CheckingTheSky();
};

#endif // CUSTOMIZEDVIEW_H
