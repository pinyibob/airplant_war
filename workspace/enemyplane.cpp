#include "enemyplane.h"
#include <QPixmap>
enemyPlane::enemyPlane()
{
    //敌机资源加载
    m_enemy.load(ENEMY_PATH);
    //敌机位置
    m_X = 0;
    m_Y = 0;

    //敌机状态
    m_Free = true;
    //敌机速度
    m_Speed = ENEMY_SPEED;
    //敌机矩形
    m_Rect.setWidth(m_enemy.width());
    m_Rect.setHeight(m_enemy.height());
    m_Rect.moveTo(m_X,m_Y);
}
void enemyPlane::updatePosition()  //用作碰撞判断的矩形一直在该类型函数中
{
    //空闲状态，不计算坐标
    if(m_Free)
    {
         return;
    }

    m_Y += m_Speed;

    m_Rect.moveTo(m_X,m_Y);

    if(m_Y >= GAME_HEIGHT)//大于边界则回收飞机进入数组
    {
       m_Free = true;
    }
}
