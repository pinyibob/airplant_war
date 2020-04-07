#ifndef MYPLANE_H
#define MYPLANE_H
#include <QPixmap>
#include "bullet.h"
class MyPlane
{
public:
    MyPlane();
    void shoot();
    void setPosition(int x,int y);

    QPixmap m_Plane;

    int m_X;
    int m_Y;

    QRect m_Rect;//飞机的矩形边框


    //使用数组管理子弹
    bullet m_bullets[BULLET_NUM];
    //发射间隔记录
    int m_recorder;
};

#endif // MYPLANE_H
