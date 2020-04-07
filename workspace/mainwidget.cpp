#include "mainwidget.h"
#include <QIcon>
#include <QPainter>
#include <ctime>
MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    initScene();
    playGame();
}

void MainWidget::initScene()
{
    setFixedSize(GAME_WIDTH,GAME_HEIGHT);
    setWindowTitle(GAME_TITLE);
    setWindowIcon(QIcon(GAME_ICON));
    m_Timer.setInterval(GAME_RATE);
    m_recorder = 0;
    //随机数种子
    srand((unsigned int)time(NULL));

}

MainWidget::~MainWidget()
{

}

void MainWidget::playGame()
{
    //启动定时器
    m_Timer.start();
    //监听定时器
    connect(&m_Timer,&QTimer::timeout,
            [=]()
    {
        //每过一定时间就刷一个飞机（设定了飞机的初始坐标，并设置了状态）
        enemyToScene();
        //更新游戏中元素的坐标（我方飞机，敌方飞机，子弹相对于时间的变化，timer每走一格则计算一次）
        updatePosition();
        //重新绘制图片（飞机，敌方飞机，子弹）
        update();
        //碰撞检测
        collisionDetection();
        });
        //背景音乐
        QSound::play(SOUND_BACKGROUND);
    }

void MainWidget::updatePosition()
{
    //地图背景变化，计算在map中实现
    m_map.mapPosition();

    //测试子弹代码
    //temp_bullet.m_Free = false;
   // temp_bullet.updatePosition();


    //发射子弹，并计算每隔一段时间发出的子弹的相应位置
    m_hero.shoot();  //该函数在一定的时间间隔内给出一个空闲子弹的初始位置

    for(int i = 0 ;i < BULLET_NUM;i++)    //该语句计算每一个发射出的子弹的变化的坐标
    {

        if(!m_hero.m_bullets[i].m_Free)
        {
              m_hero.m_bullets[i].updatePosition(); //调用bullet类中的改变坐标的函数，规则已经输入
        }
    }


    //敌机坐标计算
    for(int i = 0 ; i< ENEMY_NUM;i++)
    {
         //非空闲敌机 更新坐标
         if(m_enemys[i].m_Free == false)
         {
              m_enemys[i].updatePosition();
         }
    }
    //计算在碰撞判定函数中被初始化准备爆炸的资源，并更新爆炸当前因该播放的下一帧画面
    for(int i = 0 ; i < BOMB_NUM;i++)
    {
        if(m_bombs[i].m_Free == false)
        {
            m_bombs[i].updateInfo();
        }
    }

}

void MainWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //绘制地图
    painter.drawPixmap(0,m_map.m_map1_posY , m_map.m_map1);
    painter.drawPixmap(0,m_map.m_map2_posY , m_map.m_map2);
    painter.drawPixmap(m_hero.m_X,m_hero.m_Y,m_hero.m_Plane);
    //测试子弹代码
   // painter.drawPixmap(temp_bullet.m_X,temp_bullet.m_Y,temp_bullet.m_Bullet);

    for(int i = 0 ;i < BULLET_NUM;i++)
    {
    //画出非空闲子弹在被updatePosition函数计算过后的子弹
        if(!m_hero.m_bullets[i].m_Free)
        {
         painter.drawPixmap(m_hero.m_bullets[i].m_X,m_hero.m_bullets[i].m_Y,m_hero.m_bullets[i].m_Bullet);
        }
    }

    //根据enemytoscense放出的的飞机和updateposition修改后的坐标 绘制敌机
    for(int i = 0 ; i< ENEMY_NUM;i++)
    {
        if(m_enemys[i].m_Free == false)
        {
             painter.drawPixmap(m_enemys[i].m_X,m_enemys[i].m_Y,m_enemys[i].m_enemy);
        }
    }


    //绘制爆炸图片
    for(int i = 0 ; i < BOMB_NUM;i++)
    {
        if(m_bombs[i].m_Free == false)
        {
            painter.drawPixmap(m_bombs[i].m_X,m_bombs[i].m_Y,m_bombs[i].m_pixArr[m_bombs[i].m_index]);
        }
    }

}

void MainWidget::mouseMoveEvent(QMouseEvent *event) //鼠标移动时间独立于timer的工作，这种坐标的获取方式也会导致飞机瞬移；
{
    int x = event->x() - m_hero.m_Rect.width()*0.5; //鼠标位置 - 飞机矩形的一半
    int y = event->y() - m_hero.m_Rect.height()*0.5;
    //边界检测
    if(x <= 0 )
    {
         x = 0;
    }
    if(x >= GAME_WIDTH - m_hero.m_Rect.width())
    {
         x = GAME_WIDTH - m_hero.m_Rect.width();
    }
    if(y <= 0)
    {
         y = 0;
    }
    if(y >= GAME_HEIGHT - m_hero.m_Rect.height())
    {
         y = GAME_HEIGHT - m_hero.m_Rect.height();
    }

    m_hero.setPosition(x,y);



}

void MainWidget::enemyToScene()
{
    m_recorder++;

    if(m_recorder < ENEMY_INTERVAL)
    {
         return;
    }
    m_recorder = 0;

    for(int i = 0 ; i< ENEMY_NUM ; i++)
    {
        if(m_enemys[i].m_Free)
         {
         //敌机空闲状态改为false
          m_enemys[i].m_Free = false;
         //设置坐标
          m_enemys[i].m_X = rand() % (GAME_WIDTH - m_enemys[i].m_Rect.width());//合适的任意横坐标
          m_enemys[i].m_Y = -m_enemys[i].m_Rect.height();
         break;
         }
    }

}

void MainWidget::collisionDetection()
{

    //遍历所有非空闲的敌机
    for(int i = 0 ;i < ENEMY_NUM;i++)
    {
      if(m_enemys[i].m_Free)
     {
         //空闲飞机 跳转下一次循环
         continue;
     }
    //遍历所有 非空闲的子弹
       for(int j = 0 ; j < BULLET_NUM;j++)
         {
            if(m_hero.m_bullets[j].m_Free)
            {
             //空闲子弹 跳转下一次循环
                   continue;
            }
             //如果子弹矩形框和敌机矩形框相交，发生碰撞，同时变为空闲状态即可
            if(m_enemys[i].m_Rect.intersects(m_hero.m_bullets[j].m_Rect))
            {
                   m_enemys[i].m_Free = true;
                   m_hero.m_bullets[j].m_Free = true;//都判定位true，就不会再更新了坐标，该数组对象也不会触发绘画了，该资源回收回
                                                        //数组
                   //播放音效
                   QSound::play(SOUND_BOMB);

                   //播放爆炸效果
                   for(int k = 0 ; k < BOMB_NUM;k++)   //找一个空闲的爆炸特效，
                   {
                    if(m_bombs[k].m_Free)
                    {
                        //爆炸状态设置为非空闲
                        m_bombs[k].m_Free = false;
                        m_bombs[k].m_X = m_enemys[i].m_X;//爆炸特效在碰撞函数中确定了爆炸坐标
                        m_bombs[k].m_Y = m_enemys[i].m_Y;
                        break;
                    }
                   }
            }
         }
    }

}
