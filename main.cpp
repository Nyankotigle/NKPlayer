#include "VideoWidget.h"
#include "MusicWidget.h"
#include "InitWidget.h"
#include <QApplication>
#include <QDebug>

QString filePathArg;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //获取命令行参数
    QStringList arguments = QCoreApplication::arguments();
    if(arguments.count()>1){
        //qDebug()<<arguments.at(1);
        //将第二个命令行参数，即传入的文件路径保存到全局变量
        filePathArg = arguments.at(1);
    }

    InitWidget initwidget;
    initwidget.show();

    return a.exec();
}
