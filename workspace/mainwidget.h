#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "config.h"
#include <QTimer>
#include "map.h"
#include <QPaintEvent>
#include <myplane.h>
#include <QMouseEvent>
#include "bullet.h"
#include "enemyplane.h"
#include "bomb.h"
#include <QSound>
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    void initScene();
    ~MainWidget();

    QTimer m_Timer;


    //启动游戏 用于启动定时器对象
    void playGame();
    //更新坐标
    void updatePosition();
    //绘图事件
    void paintEvent(QPaintEvent *event);
    //地图对象
    Map m_map;

    MyPlane m_hero;


    void mouseMoveEvent(QMouseEvent *event);

    //敌机出场
    void enemyToScene();
    //敌机数组，
    enemyPlane m_enemys[ENEMY_NUM];
    //敌机出场间隔记录
    int m_recorder;

    //碰撞事件
    void collisionDetection();

    Bomb m_bombs[BOMB_NUM];

};

#endif // MAINWIDGET_H
