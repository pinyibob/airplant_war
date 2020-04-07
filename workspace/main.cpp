#include "mainwidget.h"
#include <QApplication>
#include <QResource>
#include "config.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//注册外部的二进制资源文件
    bool isOk=QResource::registerResource(GAME_RES_PATH);
    if(isOk==true)
    {
        qDebug()<<"true";
    }
    MainWidget w;

    w.show();
return a.exec();
}
