#include "InitWidget.h"
#include <QDebug>
#include <QMessageBox>

InitWidget::InitWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlag(Qt::FramelessWindowHint);//去除窗口标题栏
    QScreen* screen = QGuiApplication::primaryScreen();

    this->setAttribute(Qt::WA_TranslucentBackground,true);//实现窗口透明
    this->setMouseTracking(true);//鼠标不按压时也可以记录鼠标轨迹
    this->setWindowIcon(QIcon(":/I/Resources/NKPlayerICON2.ico"));

    _label1 = new QLabel(this);
    _label1->setWordWrap(true);//自动换行
    _label1->setAlignment(Qt::AlignCenter);//文字居中
    _label2 = new QLabel(this);
    _label2->setWordWrap(true);//自动换行
    _label2->setAlignment(Qt::AlignCenter);//文字居中

    //根据屏幕分辨率设置窗口及控件大小
    if(screen->availableGeometry().width()<1920){
        this->resize(850,250);//窗口初始大小
        _label1->setGeometry(60,30,221,165);
        _label2->setGeometry(577,47,216,165);
        _label1->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:15px;color:white; background:transparent;"));
        _label2->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:15px;color:black; background:transparent;"));
    }
    else{
        this->resize(1700,500);//窗口初始大小
        _label1->setGeometry(120,60,442,330);
        _label2->setGeometry(1150,95,432,330);
        _label1->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:30px;color:white; background:transparent;"));
        _label2->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:30px;color:black; background:transparent;"));
    }

    QFile file0("Setting/mp");
    file0.open(QIODevice::ReadOnly);
    file0.seek(0);
    QString mplayerDir = file0.readAll();
    file0.close();
    if(mplayerDir.isEmpty()){
        QMessageBox::critical(this, QStringLiteral("资源缺失"), QStringLiteral("请将mplayer.exe的完整路径复制到Resources文件夹中的mplayer文档中"));//致命问题
        exit(0);
    }


    QFile file1("Setting/VWords");
    file1.open(QIODevice::ReadOnly);
    for(int i=0;i<file1.size();i++){
        file1.seek(i);
        if(file1.read(1)=="\n"&& i!=file1.size()-1){
            QString word = file1.readLine();
            word.resize(word.size()-2);//去掉结尾的\r\n
            vWordsList << word;
        }
    }
    file1.close();

    QFile file2("Setting/MWords");
    file2.open(QIODevice::ReadOnly);
    for(int i=0;i<file2.size();i++){
        file2.seek(i);
        if(file2.read(1)=="\n"&& i!=file2.size()-1){
            QString word = file2.readLine();
            word.resize(word.size()-2);//去掉结尾的\r\n
            mWordsList << word;
        }
    }
    file2.close();

    /*
    qDebug() << vWordsList.count();
    for(int i=0;i<vWordsList.count();i++){
        qDebug() << vWordsList.at(i);
    }
    qDebug() << mWordsList.count();
    for(int i=0;i<mWordsList.count();i++){
        qDebug() << mWordsList.at(i);
    }
    */

    QTime time = QTime::currentTime();
    qsrand(uint(time.msec() + time.second()*1000));
    _label1->setText( vWordsList.at( rand() % mWordsList.count()) );
    _label2->setText( mWordsList.at( rand() % mWordsList.count()) );


    closeTimer = new QTimer;
    connect(closeTimer, SIGNAL(timeout()), this, SLOT(slotClose()));
    closeTimer->setInterval(50);
    closeTimer->setSingleShot(true);


}

void InitWidget::slotClose()
{
    this->close();
}

void InitWidget::paintEvent(QPaintEvent *)
{
    //初次绘制窗口时，如果检测到有从命令行参数获取的路径则跳过initWidget，直接打开文件
    if(!filePathArg.isEmpty())chooseVideo();

    QPainter painter(this);
    QPixmap vedio_music;
    vedio_music.load(":/I/Resources/vedio-music1.png");
    switch (status) {
    case 0:
        vedio_music.load(":/I/Resources/vedio-music1.png");
        break;
    case 1:
        vedio_music.load(":/I/Resources/vedio-music2.png");
        break;
    case 2:
        vedio_music.load(":/I/Resources/vedio-music3.png");
        break;
    default:break;
    }

     painter.drawPixmap(0, 0, this->width(), this->height() , vedio_music);
}

void InitWidget::mousePressEvent(QMouseEvent *event)
{
    if(this->width() > 1.57*(event->pos().x()+event->pos().y())){
        chooseVideo();
    }
    if(this->width() < 1.57*(event->pos().x()+event->pos().y())){
        chooseMusic();
    }
    this->update();
    return QWidget::mousePressEvent(event);
}

void InitWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(this->width() > 1.57*(event->pos().x()+event->pos().y())){
        status=1;
    }
    if(this->width() < 1.57*(event->pos().x()+event->pos().y())){
        status=2;
    }
    this->update();
    return QWidget::mouseMoveEvent(event);
}

/*
void InitWidget::mouseReleaseEvent(QMouseEvent *event)
{
    return QWidget::mouseReleaseEvent(event);
}
*/
void InitWidget::chooseMusic()
{
    musicwidget = new MusicWidget;
    musicwidget->show();
    closeTimer->start();
}

void InitWidget::chooseVideo()
{
    videowidget = new VideoWidget;
    videowidget->show();
    closeTimer->start();
}

InitWidget::~InitWidget()
{
    delete closeTimer;
}
