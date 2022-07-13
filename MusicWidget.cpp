#include "MusicWidget.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QPainter>
#include <QtGlobal>
#include <QTime>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QMessageBox>


MusicWidget::MusicWidget(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowFlag(Qt::FramelessWindowHint);//去除窗口标题栏
    this->resize(1000,700);//窗口初始大小
    this->setAttribute(Qt::WA_TranslucentBackground,true);//实现窗口透明

    this->setMouseTracking(true);//鼠标不按压时也可以记录鼠标轨迹
    this->setWindowIcon(QIcon(":/I/Resources/NKPlayerICON2.ico"));

    QFile file0("Setting/mp");
    file0.open(QIODevice::ReadOnly);
    file0.seek(0);
    mplayerDir = file0.readAll();
    file0.close();

    QMenuBar* menuBar = this->menuBar();//获取MainWindow的menubar
    //在菜单栏上添加菜单
    _menu = menuBar->addMenu("NKPlayer");
    //在菜单上添加Action
    _menu->addAction(QStringLiteral("退出"), this, SLOT(slotExit()));
    _menu->addSeparator();//添加分割线
    _menu->addAction(QStringLiteral("最小化"), this, SLOT(slotMini()));
    _menu->addSeparator();//添加分割线
    _menu->addAction(QStringLiteral("切换样式"), this, SLOT(slotChangeStyle()));
    _menu->addSeparator();//添加分割线
    _menu->addAction(QStringLiteral("使用说明:"));
    _menu->addAction(QStringLiteral("播放/暂停 Space"));
    _menu->addAction(QStringLiteral("音量+/- UP/DOWN"));
    _menu->addAction(QStringLiteral("上一首 LEFT"));
    _menu->addAction(QStringLiteral("下一首 RIGHT"));
    _menu->addAction(QStringLiteral("打开文件夹 F"));
    _menu->addAction(QStringLiteral("退出 Esc"));
    _menu->addSeparator();//添加分割线
    _menu->addAction(QStringLiteral("有问题联系:"));
    _menu->addAction(QStringLiteral("1204219458@qq.com"));
    menuBar->setVisible(false);//隐藏菜单栏


    _label1 = new QLabel(QStringLiteral("正在播放：\nxxxx-xxxx\n搜索："),this);
    _label1->setWordWrap(true);//自动换行
    _label1->setAlignment(Qt::AlignCenter);//文字居中

    _serchMusic = new QLineEdit(this);
    _serchMusic->setVisible(false);

    _slider1 = new QSlider(Qt::Horizontal,this);
    _slider1->setRange( 0,100 );
    _slider1->setValue(50);
    _slider1->setFocusPolicy(Qt::NoFocus);

    _slider2 = new QSlider(Qt::Horizontal,this);
    _slider2->setRange( 0, 2 );
    _slider2->setValue(1);
    _slider2->setFocusPolicy(Qt::NoFocus);

    _label4 = new QLabel("00:00",this);
    _label5 = new QLabel("99:99",this);
    _slider3 = new QSlider(Qt::Horizontal,this);
    _slider3->setRange( 0,100 );
    _slider3->setFocusPolicy(Qt::NoFocus);

    _slider4 = new QSlider(Qt::Vertical ,this);//歌曲列表滚动条
    _slider4->setFocusPolicy(Qt::NoFocus);

    _button1 = new QPushButton(this);
    _button1->setFocusPolicy(Qt::NoFocus);
    _button2 = new QPushButton(this);
    _button2->setFocusPolicy(Qt::NoFocus);
    _button3 = new QPushButton(this);
    _button3->setFocusPolicy(Qt::NoFocus);
    _button4 = new QPushButton(this);
    _button4->setFocusPolicy(Qt::NoFocus);


    _button5 = new QPushButton(this);
    _button5->setFocusPolicy(Qt::NoFocus);
    _button6 = new QPushButton(this);
    _button6->setFocusPolicy(Qt::NoFocus);
    _button7 = new QPushButton(this);
    _button7->setFocusPolicy(Qt::NoFocus);
    _button8 = new QPushButton(this);
    _button8->setFocusPolicy(Qt::NoFocus);
    _button9 = new QPushButton(this);
    _button9->setFocusPolicy(Qt::NoFocus);
    _button10 = new QPushButton(this);
    _button10->setFocusPolicy(Qt::NoFocus);
    _button11 = new QPushButton(this);
    _button11->setFocusPolicy(Qt::NoFocus);


    connect(_serchMusic,SIGNAL(textChanged(const QString &)),this,SLOT(slotSerchMusic(const QString &)));
    connect(_button1, SIGNAL(clicked()), this, SLOT(slotButton1Clicked()));
    connect(_button2, SIGNAL(clicked()), this, SLOT(slotButton2Clicked()));
    connect(_button3, SIGNAL(clicked()), this, SLOT(slotButton3Clicked()));
    connect(_button4, SIGNAL(clicked()), this, SLOT(slotButton4Clicked()));
    connect(_button5, SIGNAL(clicked()), this, SLOT(slotButton5_11Clicked()));
    connect(_button6, SIGNAL(clicked()), this, SLOT(slotButton5_11Clicked()));
    connect(_button7, SIGNAL(clicked()), this, SLOT(slotButton5_11Clicked()));
    connect(_button8, SIGNAL(clicked()), this, SLOT(slotButton5_11Clicked()));
    connect(_button9, SIGNAL(clicked()), this, SLOT(slotButton5_11Clicked()));
    connect(_button10, SIGNAL(clicked()), this, SLOT(slotButton5_11Clicked()));
    connect(_button11, SIGNAL(clicked()), this, SLOT(slotButton5_11Clicked()));
    connect(_slider3, SIGNAL(sliderReleased()), this , SLOT(slotOnSlider3Released()));
    connect(_slider3, SIGNAL(sliderPressed()), this , SLOT(slotOnslider3Pressed()));
    connect(_slider3, SIGNAL(valueChanged(int)), this , SLOT(slotOnslider3ValueChanged(int)));
    connect(_slider4, SIGNAL(valueChanged(int)), this , SLOT(slotOnslider4ValueChanged(int)));
    connect(_slider1, SIGNAL(valueChanged(int)), this , SLOT(slotOnSlider1Changed(int)));

    //读取上次样式
    QFile file("Setting/MSetting");
    file.open(QIODevice::ReadOnly);
    file.seek(400);
    nowStyleKind = file.read(1).toInt();
    file.close();
    //设置样式
    SetStyle();

    slotButton4Clicked();//读取文件夹,并初始化列表

    //每隔500ms，查询一次视频当前播放的时间
    timerGetTime = new QTimer;
    connect(timerGetTime, SIGNAL(timeout()), this, SLOT(slotGetTime()));
    timerGetTime->setInterval(500);
    timerGetTime->start();

    //延迟500ms后再开始查询时间，避免手动调整滑块位置后，播放时间没能及时更新
    timerDelayGetTime = new QTimer;
    connect(timerDelayGetTime, SIGNAL(timeout()), this, SLOT(slotTimerDelayGetTimeOut()));
    timerDelayGetTime->setInterval(500);
    timerDelayGetTime->setSingleShot(true);

    //重绘计时器
    rePaintTimer = new QTimer();
    rePaintTimer->setInterval(50);//设置定时时间为1秒
    //将定时器时间结束信号与slotTimeout槽函数连接起来
    connect(rePaintTimer, SIGNAL(timeout()), this ,SLOT(update()));
    rePaintTimer->start();//定时器开始

}

bool MusicWidget::eventFilter(QObject * object, QEvent * event)
{
    /*
    if(object == (QObject*)_serchMusic && (!_serchMusic->isVisible())){
    }
    */
    return QWidget::eventFilter(object, event);//回到原来的流程上
}

void MusicWidget::slotExit()
{
    this->close();
}
void MusicWidget::slotMini()
{
    this->showMinimized();//最小化
}

//搜索歌曲
void MusicWidget::slotSerchMusic(const QString &)
{
    //如果搜索栏字符为空，返回
    if(_serchMusic->text().isEmpty())return;

    QString nowPlayStr;
    //从歌曲列表中匹配搜索栏中的歌曲名
    for(int i=0;i<musicNameList.count();i++){
        if(musicNameList.at(i) == _serchMusic->text()){
            //如果匹配到了，并且当前有播放进程，则将其关闭
            if(MusicProcess.isOpen())MusicProcess.close();
            //更新Nowplay为搜索到的序号
            listShowMid = nowPlay = i;
            nowPlayStr = musicPathList.at(nowPlay);
            //确保歌曲路径不为空
            if(nowPlayStr.isEmpty())return;
            //若路径不为空，加入播放记录
            else playRecord << nowPlay;
            //对于样式0和4，nowPlay改变后立刻更新歌曲列表
            if(nowStyleKind==0||nowStyleKind==4)ShowList();
            QStringList args;
            args << "-slave";//使用管道命令
            args << "-quiet";
            args << "-zoom";
            args << nowPlayStr;
            MusicProcess.start(mplayerDir, args);
            MusicProcess.write(QString("volume 50 1\n").toUtf8());
            _label1->setText(musicNameList.at(nowPlay));
            if(isPause)isPause=1-isPause;
        }
    }
}

//更新时间
void MusicWidget::slotGetTime()
{
    //如果进程未启动，退出
    if(!MusicProcess.isOpen())return;
    if(isPause||slider3_isPressed||key_RL_isPressed) return;

    MusicProcess.write(QString("get_time_length\n").toUtf8());//获取总时间
    MusicProcess.write(QString("get_time_pos\n").toUtf8());//获取当前时间
    QByteArray gettime = MusicProcess.readAll();//读取结果

    //当通过拖动滑块，左右方向键调整视频进度后，紧接着查询的当前播放信息尚未更新
    //需要查询几次后才会更新结果，此时的查询结果不可用
    //解决了当手动调整视频进度后，进度条和左侧当前时间闪回到调整前的值的问题
    if(canReadTimeValue==true){
        //从结果字符串中提取出两个时间的字符串
        QByteArray timeAll, timeNow;
        bool isGet = false;
        bool gotAll = false;
        for(int i=0;i<gettime.size();i++)
        {
            if(gettime.at(i)=='='){
                isGet = true;
                continue;
            }
            if(gettime.at(i)=='.'){
                isGet = false;
                gotAll = true;
                continue;
            }
            if(isGet==true && gotAll==false){
                timeAll += gettime.at(i);
            }
            if(isGet==true && gotAll==true){
                timeNow += gettime.at(i);
            }
        }
        //qDebug() <<timeAll.toInt()<<" , "<< timeNow.toInt();

        //当所获取的总秒数和当前秒数都不为0时，显示并记录相应的值
        //避免有时获取失败得到的值为0，尤其当使用滑块调整进度时
        if(timeAll.toInt()!=0 && timeNow.toInt()!=0){
            //将提取出的以秒为单位的时间字符串，转换为"分钟:秒钟"格式字符串
            QString showNowTime, showAllTime;
            showNowTime += QString::number(timeNow.toInt()/60);
            showNowTime += ":";
            showNowTime += QString::number(timeNow.toInt()%60);
            showAllTime += QString::number(timeAll.toInt()/60);
            showAllTime += ":";
            showAllTime += QString::number(timeAll.toInt()%60);
            //将最终得到的时间字符串显示在两个label上
            _label4->setText(showNowTime);
            _label5->setText(showAllTime);
            //当获取的记录整数类型的总秒数、当前秒数
            secAll = timeAll.toInt();
            secNow = timeNow.toInt();
            //将进度条的最大值设为总秒数
            _slider3->setRange(0,secAll);
            _slider3->setValue(secNow);
            //当前秒数距离总秒数一定值后，自动播放下一首
            if(secNow >= secAll-2 && secAll!=0){
                slotButton3Clicked();
                _slider3->setValue(0);
            }
        }
    }
}
void MusicWidget::slotTimerDelayGetTimeOut()
{
    canReadTimeValue = true;//开始读取查询结果中的时间
}

void MusicWidget::slotOnSlider3Released()
{
    slider3_isPressed = false;//鼠标释放滑块
    //如果释放滑块时，滑块已经到了最右边，则直接视为播放下一首，并将滑块的值置0
    if(_slider3->value() >= secAll-2){
        slotButton3Clicked();
        _slider3->setValue(0);
    }
    //否则定位到滑块的位置播放
    else MusicProcess.write(QString("seek %1 2\n").arg(_slider3->value()).toUtf8());
    //延迟500ms后再开始读取查询结果中的时间
    timerDelayGetTime->start();

    //如果调整播放进度时，视频为暂停状态，则将其切换为播放状态
    //否则当进度调整完毕时，视频自动变为播放状态，但此时记录的仍然是暂停状态，导致时间不更新
    if(isPause){
        MusicProcess.write(QString("pause\n").toUtf8());
        isPause = 1-isPause;//暂停\播放状态切换
    }
}
void MusicWidget::slotOnslider3Pressed()
{
    slider3_isPressed = true;//鼠标按下滑块
    canReadTimeValue = false;//暂停读取查询结果中的时间
}
void MusicWidget::slotOnslider3ValueChanged(int value)
{
    //当鼠标拖动滑块使得滑块的值改变时，左侧的当前时间跟着变化
    if(slider3_isPressed){
        QString showNowTime;
        showNowTime += QString::number(value/60);
        showNowTime += ":";
        showNowTime += QString::number(value%60);
        _label4->setText(showNowTime);
    }    
}

void MusicWidget::slotOnslider4ValueChanged(int value)
{
    //拖动歌曲列表滚动条，刷新歌曲列表
    listShowMid = value;
    ShowList();
}


//播放、暂停
void MusicWidget::slotButton1Clicked()
{
    //如果当前没有播放进程，即刚打开程序，则从文件记录的nowPaly开始播放
    if(!MusicProcess.isOpen()){
        QString nowPlayStr;
        if(nowPlay < musicPathList.count()){//防止nowplay和列表不匹配
            nowPlayStr=musicPathList.at(nowPlay);
            playRecord << nowPlay;//将当前播放序号加入记录队列
        }
        if(nowPlayStr.isEmpty())return;//路径为空，则退出函数
        QStringList args;
        args << "-slave";//使用管道命令
        args << "-quiet";
        args << "-zoom";
        args << nowPlayStr;
        MusicProcess.start(mplayerDir, args);
        MusicProcess.write(QString("volume 50 1\n").toUtf8());
        _label1->setText(musicNameList.at(nowPlay));
        if(isPause)isPause=1-isPause;//确保转换到播放状态
    }
    else {
        //如果当前有播放进程，则只用切换播放状态
        MusicProcess.write(QString("pause\n").toUtf8());//暂停\播放
        isPause = 1-isPause;//暂停\播放状态切换
    }
}

//上一首
void MusicWidget::slotButton2Clicked()
{
    //如果当前有播放进程，则将其关闭；如果当前没有播放进程，则播放记录为空，退出函数
    if(MusicProcess.isOpen())MusicProcess.close();
    else return;

    QString nowPlayStr;
    //如果模式为单曲，则nowplay不变
    if(_slider2->value()==0 && nowPlay < musicPathList.count())
        nowPlayStr=musicPathList.at(nowPlay);
    //如果模式为列表，则mowplay-1并且小于0的话，回滚到最后第一首
    if(_slider2->value()==2 && nowPlay < musicPathList.count()){
        listShowMid = nowPlay = (musicPathList.count()+nowPlay-1) % musicPathList.count();
        nowPlayStr=musicPathList.at(nowPlay);
    }
    //如果模式为随机
    if(_slider2->value()==1 && nowPlay < musicPathList.count()){
        backONrecord++;
        //若当前已经是播放记录的第一首
        if(playRecord.size()-backONrecord < 1){
            backONrecord--;
            return;
        }
        listShowMid = nowPlay = playRecord.at(playRecord.size()- backONrecord-1);
        nowPlayStr=musicPathList.at(nowPlay);
    }

    //对于样式0和4，nowPlay改变后立刻更新歌曲列表
    if(nowStyleKind==0||nowStyleKind==4)ShowList();

    if(nowPlayStr.isEmpty())return;//确保路径不为空
    QStringList args;
    args << "-slave";//使用管道命令
    args << "-quiet";
    args << "-zoom";
    args << nowPlayStr;
    MusicProcess.start(mplayerDir, args);
    MusicProcess.write(QString("volume 50 1\n").toUtf8());
    _label1->setText(musicNameList.at(nowPlay));
    if(isPause)isPause=1-isPause;//确保切换歌曲后，状态为播放
}

//下一首
void MusicWidget::slotButton3Clicked()
{
    //如果当前有播放进程，将其关闭
    if(MusicProcess.isOpen())MusicProcess.close();
    QString nowPlayStr;
    //如果模式为单曲，则nowplay不变
    if(_slider2->value()==0 && nowPlay < musicPathList.count())
        nowPlayStr=musicPathList.at(nowPlay);
    //如果模式为随机
    if(_slider2->value()==1 && nowPlay < musicPathList.count()){
        //并且当前不是在播放记录中，则在当前歌曲序号中产生一个随机序号作为下一首
        if(backONrecord == 0){
            //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//产生的随机数太靠近
            QTime time = QTime::currentTime();
            qsrand(uint(time.msec() + time.second()*1000));
            listShowMid = nowPlay = rand() % musicPathList.count();
            //qDebug()<<nowPlay;
            nowPlayStr=musicPathList.at(nowPlay);
            playRecord << nowPlay;//只有在随机播放模式下才有加入播放记录的必要
        }
        //如果当前是在播放记录中，则顺序播放记录中的下一首
        if(backONrecord > 0){
            backONrecord--;
            listShowMid = nowPlay = playRecord.at(playRecord.size()-backONrecord-1);
            nowPlayStr=musicPathList.at(nowPlay);
        }
    }
    //如果模式为列表，则mowplay+1并且超出歌曲总数范围的话，回滚到第一首
    if(_slider2->value()==2 && nowPlay < musicPathList.count()){
        listShowMid = nowPlay = (nowPlay+1) % musicPathList.count();
        nowPlayStr=musicPathList.at(nowPlay);
    }

    //对于样式0和4，nowPlay改变后立刻更新歌曲列表
    if(nowStyleKind==0||nowStyleKind==4)ShowList();
    if(nowPlayStr.isEmpty())return;//确保路径不为空

    QStringList args;
    args << "-slave";//使用管道命令
    args << "-quiet";
    args << "-zoom";
    args << nowPlayStr;
    MusicProcess.start(mplayerDir, args);
    MusicProcess.write(QString("volume 50 1\n").toUtf8());
    //凡是在MusicProcess.start后面都要及时把歌名显示在label1上
    _label1->setText(musicNameList.at(nowPlay));
    //凡是在MusicProcess.start后面都确保isPause为false,因为start会自动开始播放
    if(isPause)isPause=1-isPause;
}

//设置歌曲文件夹
void MusicWidget::slotButton4Clicked()
{
    bool initMusicFolder;//是否是情况1
    //情况1：歌曲路径列表为空，即程序刚打开，此时需要从文件读取歌曲列表，若文件为空，则选择歌曲文件夹
    if(musicPathList.isEmpty())
    {
        initMusicFolder = true;
        QFile file("Setting/MSetting");//指定配置文件
        file.open(QIODevice::ReadWrite);//读写模式打开指定的文件
        file.seek(0);
        //如果第0个字节为空，即文件中没有保存nowplay
        if(file.read(1).isEmpty()) nowPlay = listShowMid = 0;//将其设为0
        //文件中的nowplay不为空
        else{
            file.seek(0);
            //读取nowplay这一行
            QString strnowplay = file.readLine();
            //去掉结尾的换行符
            strnowplay.resize(strnowplay.size()-1);
            //将读取的字符串转换为int值，并更新listShowMid和nowplay
            listShowMid = nowPlay = strnowplay.toInt();
            //qDebug()<<nowPlay;
        }
        file.seek(100);
        //如果文件中的文件夹路径为空
        if(file.read(1).isEmpty()){
            //选择文件夹
            _strMusicFolder = QFileDialog::getExistingDirectory(
                        this,
                        QStringLiteral("选择歌曲所在的文件夹"),
                        "/",
                        QFileDialog::ShowDirsOnly);
            if(_strMusicFolder.isEmpty()) exit(0);//如果路径为空则退出程序
        }
        //如果文件中的路径不为空，则读取
        else {
            file.seek(100);
            _strMusicFolder = file.readLine();
            _strMusicFolder.resize(_strMusicFolder.size()-1);
        }
        file.close();//关闭文件
    }

    //情况2：如果歌曲列表不为空，即程序已经加载过歌曲文件，这时是为了更换歌曲文件夹
    else
    {
        initMusicFolder = false;
        //选择文件夹
        _strMusicFolder = QFileDialog::getExistingDirectory(
                    this,
                    QStringLiteral("选择歌曲所在的文件夹"),
                    "/",
                    QFileDialog::ShowDirsOnly);
        //如果选择的路径为空
        if(_strMusicFolder.isEmpty()){qDebug()<<"empty folder"; return;}
    }

    QDir dir(_strMusicFolder);
    dir.setFilter(QDir::Files);//设置过滤器：只筛选文件，不筛选文件夹
    QFileInfoList fileInfoList = dir.entryInfoList();//获取文件列表
    //若文件列表为空
    if(fileInfoList.isEmpty()){
        QMessageBox::critical(this, QStringLiteral("未找到音频文件"), QStringLiteral("请重新选择文件夹！"));
        return;
    }
    //先创建临时列表存储读取的音频文件,这样当选择的文件夹中没有音频文件时，record和nowplay不变，不会影响当前播放进程
    QStringList musicPathListTemp , musicNameListTemp;
    for(int i=0; i<fileInfoList.count(); i++){
        //筛选文件后缀
        if(fileInfoList.at(i).suffix()=="mp3"||fileInfoList.at(i).suffix()=="wma"
                ||fileInfoList.at(i).suffix()=="flac"||fileInfoList.at(i).suffix()=="aac"
                ||fileInfoList.at(i).suffix()=="wav"){
            musicPathListTemp << fileInfoList.at(i).filePath();//文件完整路径
            musicNameListTemp << fileInfoList.at(i).fileName();//文件名加后缀
            //musicNameList << fileInfoList.at(i).baseName();//文件名无后缀
            //这里如果用文件名无后缀baseName，当文件名中有'.'时获取的文件名不完整
        }
    }
    //若临时列表为空，即文件列表中没有音频文件
    if(musicPathListTemp.isEmpty()){
        QMessageBox::critical(this, QStringLiteral("未找到音频文件"), QStringLiteral("请重新选择文件夹！"));
        return;
    }
    else {
        musicPathList = musicPathListTemp;
        musicNameList = musicNameListTemp;
        //如果是情况2，还要有以下操作
        if(!initMusicFolder){
            //如果当前有歌曲在播放则将其关闭
            if(MusicProcess.isOpen()) MusicProcess.close();
            //将临时列表赋给当前歌曲列表，清空播放记录，并且重置listShowMid和nowplay
            playRecord.clear();
            listShowMid = nowPlay  = 0;
        }
    }

    //对于样式0和4，加载文件后即显示歌曲列表
    if(nowStyleKind==0||nowStyleKind==4)ShowList();

    //根据文件列表构造搜索栏的提示列表
    musicNameCptr = new QCompleter(musicNameList);
    musicNameCptr->setFilterMode(Qt::MatchContains);//设置筛选模式为包含输入字符
    musicNameCptr->setMaxVisibleItems(17);//最大显示17个提示
   _serchMusic->setCompleter(musicNameCptr);   

   _slider4->setRange( 0, musicNameList.size());//设置歌曲列表滚动条范围

   _label1->setText(musicNameList.at(nowPlay));//更新当前播放

   QFile file("Setting/MSetting");//指定文件
   file.open(QIODevice::ReadWrite);//读写模式打开指定的文件
   file.seek(100);//将新获取的文件夹路径保存到文件
   QString folder;
   folder += _strMusicFolder;
   folder += "\n";
   file.write(folder.toUtf8());
   file.close();
}

//点击歌曲列表
void MusicWidget::slotButton5_11Clicked()
{
    //如果当前有播放进程将其关闭
    if(MusicProcess.isOpen())MusicProcess.close();
    //将nowplay设为发送信号的button上的歌曲的对应的序号
    if(sender()==_button5) listShowMid = nowPlay = _button5Value;
    if(sender()==_button6) listShowMid = nowPlay = _button6Value;
    if(sender()==_button7) listShowMid = nowPlay = _button7Value;
    if(sender()==_button8) listShowMid = nowPlay = _button8Value;
    if(sender()==_button9) listShowMid = nowPlay = _button9Value;
    if(sender()==_button10) listShowMid = nowPlay = _button10Value;
    if(sender()==_button11) listShowMid = nowPlay = _button11Value;
    //保存到播放记录
    playRecord << nowPlay;
    QStringList args;
    args << "-slave"<< "-quiet"<< "-zoom"<<musicPathList.at(nowPlay);
    MusicProcess.start(mplayerDir, args);
    MusicProcess.write(QString("volume 50 1\n").toUtf8());
    _label1->setText(musicNameList.at(nowPlay));
    if(isPause)isPause=1-isPause;
    //对于样式0和4，nowPlay改变后立刻更新歌曲列表
    if(nowStyleKind==0||nowStyleKind==4)ShowList();
}

void MusicWidget::slotOnSlider1Changed(int value)
{
    //调整音量大小
    MusicProcess.write(QString("volume %1 1\n").arg(value).toUtf8());
}

//绘制窗口，构造界面
void MusicWidget::paintEvent(QPaintEvent *)
{
    if(nowStyleKind==0){
        //背景
        QPainter painter1(this);
        QPixmap music_widget(":/M/Resources/musicWidget.png");
        painter1.drawPixmap(0, 0, this->width(), this->height() , music_widget);

        //旋转的唱片
        QTime time = QTime::currentTime();
        QPainter painter2(this);
        QPixmap disk;
        disk.load(":/M/Resources/disk.png");
        painter2.setRenderHint((QPainter::Antialiasing));//抗锯齿
        painter2.translate(350,344);//将原点设为唱片的中心
        //每分钟转一圈，即每秒6度，为了看起来更顺滑，需要精确到毫秒
        painter2.rotate((time.second()+(time.msec()/1000.0))*12.0);
        //这里旋转的其实是painter2，而图片相对painter2的位置是固定的
        painter2.drawPixmap(QRect(-340,-340,disk.width(),disk.height()), disk);

        //探针，播放时读盘，暂停时归位
        QPainter painter3(this);
        if(isPause){
            QPixmap music_pause(":/M/Resources/musicWidgetPause.png");
            painter3.drawPixmap(0, 0, this->width(), this->height() , music_pause);
        }
        else {
            QPixmap music_play(":/M/Resources/musicWidgetPlay.png");
            painter3.drawPixmap(0, 0, this->width(), this->height() , music_play);
        }
    }
    if(nowStyleKind==1){
        QPainter painter1(this);
        QPixmap music_widget(":/M/Resources/musicWidget1.png");
        painter1.drawPixmap(0, 0, this->width(), this->height() , music_widget);
    }
    if(nowStyleKind==2){
        QPainter painter1(this);
        QPixmap music_widget(":/M/Resources/musicWidget2.png");
        painter1.drawPixmap(0, 0, this->width(), this->height() , music_widget);
    }
    if(nowStyleKind==3){
        QPainter painter1(this);
        QPixmap music_widget(":/M/Resources/musicWidget3.png");
        painter1.drawPixmap(0, 0, this->width(), this->height() , music_widget);
    }
    if(nowStyleKind==4){
        QPainter painter1(this);
        QPixmap music_widget(":/M/Resources/musicWidget4.png");
        painter1.drawPixmap(0, 0, this->width(), this->height() , music_widget);
    }
}


void MusicWidget::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();//记录鼠标按压位置
    if(event->button() == Qt::LeftButton){
        if (x >= 680 && x <= 980){
            if (y >= 0 && y <= this->height()){
                m_isPressed = true;//按压位置在右侧播放器界面
                m_startMovePos = event->pos();//将鼠标按下位置作为窗口移动的起始坐标
            }
        }
    }

    /*右键点击在点击位置显示菜单*/
    if(event->button() == Qt::RightButton)
        _menu->exec(QCursor::pos());//在当前光标位置将_menu作为模态对话框显示
    //QCursor中的pos()是当前鼠标相对整个屏幕的位置，而QMouseEvent中的pos()是鼠标在当前窗口中的位置

    return QWidget::mousePressEvent(event);
}
void MusicWidget::mouseMoveEvent(QMouseEvent *event)
{
    //当鼠标按在播放器界面
    if (m_isPressed)
    {
        QPoint movePoint = event->globalPos();//记录鼠标当前移动到的位置
        //窗口在x,y轴分量的的移动距离分别为movePoint和m_startMovePos坐标分量的差值
        this->move(movePoint.x() - m_startMovePos.x(), movePoint.y() - m_startMovePos.y());
    }
    //记录鼠标移动到的位置
    int x = event->pos().x();
    int y = event->pos().y();
    /*
    //如果鼠标离开唱片区域，隐藏唱片上的歌曲列表
    if (x < 0 || x > 680 || y < 0 || y > this->height())
    {
        _button5->setVisible(false);
        _button6->setVisible(false);
        _button7->setVisible(false);
        _button8->setVisible(false);
        _button9->setVisible(false);
        _button10->setVisible(false);
        _button11->setVisible(false);
    }
    */
    //如果鼠标移动到播放器上的显示屏界面，显示搜索栏，否则隐藏搜索栏
    if((x>700 && x<960 && y>100 && y<260 && nowStyleKind!=3)||
            (x>586 && x<846 && y>100 && y<260 && nowStyleKind==3)){
        _serchMusic->setVisible(true);
        _serchMusic->setEnabled(true);
    }
    else{
        _serchMusic->clear();//清除文字
        _serchMusic->setVisible(false);//隐藏搜索栏
        _serchMusic->setEnabled(false);//暂时不可用，让其不响应任何消息，以免搜索栏一直拦截键盘消息
    }

    return QWidget::mouseMoveEvent(event);
}
void MusicWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;//鼠标释放
    return QWidget::mouseReleaseEvent(event);
}

void MusicWidget::wheelEvent(QWheelEvent* event)
{
    //但样式为123时，不响应滚轮消息
    if(nowStyleKind==1||nowStyleKind==2||nowStyleKind==3) return QWidget::wheelEvent(event);

    /*
    //记录滚轮滚动时鼠标位置
    int x = event->pos().x();
    int y = event->pos().y();
    //鼠标在唱片区域，显示唱片上的歌曲列表
    if (x >= 0 && x <= 680 && y >= 0 && y <= this->height())
    {
        _button5->setVisible(true);
        _button6->setVisible(true);
        _button7->setVisible(true);
        _button8->setVisible(true);
        _button9->setVisible(true);
        _button10->setVisible(true);
        _button11->setVisible(true);
    }
    */

    if(event->delta()<0)listShowMid++;
    if(event->delta()>0)listShowMid--;
    //根据滚轮的滚动方向增加或减小列表序号保存到相应的buttonValue中，并且更新列表上的歌曲名称
    ShowList();

    return QWidget::wheelEvent(event);
}
void MusicWidget::ShowList()//将ShowList分离出来，以便初始未转动滚轮时也可显示列表
{
    _button5->setVisible(true);
    _button6->setVisible(true);
    _button7->setVisible(true);
    _button8->setVisible(true);
    _button9->setVisible(true);
    _button10->setVisible(true);
    _button11->setVisible(true);
    if(musicNameList.count()>7){
        //如果listShowMid大于总数，则取余
        if(listShowMid>=musicNameList.count()) listShowMid %= musicNameList.count();
        //如果listShowMid小于零，则加上总数
        if(listShowMid<0)listShowMid= listShowMid+musicNameList.count();

        //比listShowMid小的要防止下溢
        _button5->setText(musicNameList.at( (listShowMid-3+musicNameList.count())%musicNameList.count() ).toUtf8());
        _button5Value =  (listShowMid-3+musicNameList.count())%musicNameList.count() ;
        _button6->setText(musicNameList.at( (listShowMid-2+musicNameList.count())%musicNameList.count() ).toUtf8());
        _button6Value =  (listShowMid-2+musicNameList.count())%musicNameList.count() ;
        _button7->setText(musicNameList.at( (listShowMid-1+musicNameList.count())%musicNameList.count() ).toUtf8());
        _button7Value =  (listShowMid-1+musicNameList.count())%musicNameList.count() ;
        //listShowMid放在中间
        _button8->setText(musicNameList.at(listShowMid).toUtf8());_button8Value = listShowMid;
        //比listShowMid大的要防止上溢
        _button9->setText(musicNameList.at((listShowMid+1)%musicNameList.count()).toUtf8());
        _button9Value = (listShowMid+1)%musicNameList.count();
        _button10->setText(musicNameList.at((listShowMid+2)%musicNameList.count()).toUtf8());
        _button10Value = (listShowMid+2)%musicNameList.count();
        _button11->setText(musicNameList.at((listShowMid+3)%musicNameList.count()).toUtf8());
        _button11Value = (listShowMid+3)%musicNameList.count();
    }
    //当歌曲总数小于等于7时单独设置列表显示顺序，使得歌曲总显示在列表中间，并且隐藏没有歌曲的button
    if(musicNameList.count()==0){
        _button5->setVisible(false);
        _button6->setVisible(false);
        _button7->setVisible(false);
        _button8->setVisible(false);
        _button9->setVisible(false);
        _button10->setVisible(false);
        _button11->setVisible(false);
    }
    if(musicNameList.count()==1){
        _button8->setText(musicNameList.at(0));_button8Value = 0;
        _button5->setVisible(false);
        _button6->setVisible(false);
        _button7->setVisible(false);
        _button9->setVisible(false);
        _button10->setVisible(false);
        _button11->setVisible(false);
    }
    if(musicNameList.count()==2){
        _button7->setText(musicNameList.at(0));_button7Value = 0;
        _button8->setText(musicNameList.at(1));_button8Value = 1;
        _button5->setVisible(false);
        _button6->setVisible(false);
        _button9->setVisible(false);
        _button10->setVisible(false);
        _button11->setVisible(false);
    }
    if(musicNameList.count()==3){
        _button7->setText(musicNameList.at(0));_button7Value = 0;
        _button8->setText(musicNameList.at(1));_button8Value = 1;
        _button9->setText(musicNameList.at(2));_button9Value = 2;
        _button5->setVisible(false);
        _button6->setVisible(false);
        _button10->setVisible(false);
        _button11->setVisible(false);
    }
    if(musicNameList.count()==4){
        _button6->setText(musicNameList.at(0));_button6Value = 0;
        _button7->setText(musicNameList.at(1));_button7Value = 1;
        _button8->setText(musicNameList.at(2));_button8Value = 2;
        _button9->setText(musicNameList.at(3));_button9Value = 3;
        _button5->setVisible(false);
        _button10->setVisible(false);
        _button11->setVisible(false);
    }
    if(musicNameList.count()==5){
        _button6->setText(musicNameList.at(0));_button6Value = 0;
        _button7->setText(musicNameList.at(1));_button7Value = 1;
        _button8->setText(musicNameList.at(2));_button8Value = 2;
        _button9->setText(musicNameList.at(3));_button9Value = 3;
        _button10->setText(musicNameList.at(4));_button10Value = 4;
        _button5->setVisible(false);
        _button11->setVisible(false);
    }
    if(musicNameList.count()==6){
        _button5->setText(musicNameList.at(0));_button5Value = 0;
        _button6->setText(musicNameList.at(1));_button6Value = 1;
        _button7->setText(musicNameList.at(2));_button7Value = 2;
        _button8->setText(musicNameList.at(3));_button8Value = 3;
        _button9->setText(musicNameList.at(4));_button9Value = 4;
        _button10->setText(musicNameList.at(5));_button10Value = 5;
        _button11->setVisible(false);
    }
    if(musicNameList.count()==7){
        _button5->setText(musicNameList.at(0));_button5Value = 0;
        _button6->setText(musicNameList.at(1));_button6Value = 1;
        _button7->setText(musicNameList.at(2));_button7Value = 2;
        _button8->setText(musicNameList.at(3));_button8Value = 3;
        _button9->setText(musicNameList.at(4));_button9Value = 4;
        _button10->setText(musicNameList.at(5));_button10Value = 5;
        _button11->setText(musicNameList.at(6));_button11Value = 6;
    }
}


void MusicWidget::keyPressEvent(QKeyEvent * ev)
{
    int key = ev->key();
    //如果按下Esc键
    if(key==16777216) this->close();//退出
    //空格键暂停\播放
    if(key==32) slotButton1Clicked();
    //F打开文件
    if(key==70) slotButton4Clicked();
    //Left上一首
    if(key==16777234) slotButton2Clicked();
    //Right下一首
    if(key==16777236) slotButton3Clicked();
    //Up音量加
    if(key==16777235) _slider1->setValue(_slider1->value()+3);
    //Down音量减
    if(key==16777237) _slider1->setValue(_slider1->value()-3);

    //qDebug()<<key;
    return QWidget::keyPressEvent(ev);
}


void MusicWidget::closeEvent(QCloseEvent *event)
{
    //销毁进程
    MusicProcess.kill();
    //关闭之前保存
    QFile file("Setting/MSetting");//指定文件
    file.open(QIODevice::ReadWrite);//读写模式打开指定的文件
    //将当前播放序号转换为字符串从第0字节开始保存，并在结尾加上换行符表示结束
    file.seek(0);
    QString strnowplsy = QString::number(nowPlay);
    strnowplsy += "\n";
    file.write(strnowplsy.toUtf8());
    //共预留了100个字节，在第99个字节写入换行符，方便人工检查文件
    file.seek(99);
    file.write("\n");
    file.seek(399);
    file.write("\n");
    //在第400个字节写入样式信息
    file.seek(400);
    file.write(QByteArray::number(nowStyleKind));
    file.seek(401);
    file.write("\n");
    file.close();
    return QWidget::closeEvent(event);
}

void MusicWidget::slotChangeStyle()
{
    nowStyleKind = (++nowStyleKind) % 5; //0,1,2,3,4循环
    SetStyle();
}
void MusicWidget::SetStyle()
{
    //控件样式
    switch (nowStyleKind) {
    case 0:
        _menu->setStyleSheet("font-family:'华文中宋';font:bold;font-size:20px;background-color:rgb(200,115,50);");
        _label1->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:23px;color:black; background:rgb(100,230,150);"));
        _serchMusic->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:23px;color:black; background-color:rgb(100,230,150);border:1px solid balck"));
        _slider1->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:black; height:4px;}QSlider::groove:Horizontal{background:transparent;height:6px;}QSlider::handle:Horizontal{height: 28px;width:28px;border-image: url(:/M/Resources/sliderHandle.png);margin: -12 0px;}");
        _slider2->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:black; height:4px;}QSlider::groove:Horizontal{background:transparent;height:6px;}QSlider::handle:Horizontal{height: 28px;width:28px;border-image: url(:/M/Resources/sliderHandle.png);margin: -12 0px;}");
        _slider3->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:black; height:4px;}QSlider::groove:Horizontal{background:transparent;height:6px;}QSlider::handle:Horizontal{height: 28px;width:28px;border-image: url(:/M/Resources/sliderHandle.png);margin: -12 0px;}");
        _label4->setStyleSheet(QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:black; background-color:transparent;"));
        _label5->setStyleSheet(QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:black; background-color:transparent;"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(50,230,250,150)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(50,230,250,150)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(50,230,250,150)}"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(50,230,250,150)}"));
        _button5->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:20px;font:bold;color:white;} QPushButton{border-radius:25px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button6->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:25px;font:bold;color:white;} QPushButton{border-radius:30px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button7->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:30px;font:bold;color:white;} QPushButton{border-radius:35px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button8->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:35px;color:white;} QPushButton{border-radius:40px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button9->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:30px;font:bold;color:white;} QPushButton{border-radius:35px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button10->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:25px;font:bold;color:white;} QPushButton{border-radius:30px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button11->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:20px;font:bold;color:white;} QPushButton{border-radius:25px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        break;

    case 1:
        _menu->setStyleSheet("font-family:'华文中宋';font:bold;font-size:20px;background-color:rgb(200,115,50);");
        _label1->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:23px;color:black; background:transparent;"));
        _serchMusic->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:23px;color:black; background-color:transparent;border:1px solid balck"));
        _slider1->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:black; height:4px;}QSlider::groove:Horizontal{background:transparent;height:6px;}QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;border-radius:10px;margin: -8 0px;}");
        _slider2->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:black; height:4px;}QSlider::groove:Horizontal{background:transparent;height:6px;}QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;border-radius:10px;margin: -8 0px;}");
        _slider3->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:black; height:4px;}QSlider::groove:Horizontal{background:transparent;height:6px;}QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;border-radius:10px;margin: -8 0px;}");
        _label4->setStyleSheet(QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:black; background-color:transparent;"));
        _label5->setStyleSheet(QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:black; background-color:transparent;"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,70)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,70)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,70)}"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,70)}"));
        break;

    case 2:
        _menu->setStyleSheet("font-family:'华文中宋';color:rgba(133,133,133,100);font:bold;font-size:20px;background:rgb(240,240,240);selection-color: white;selection-background-color: rgba(133,133,133,100);");
        _label1->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:23px;color:rgba(133,133,133,220); background:transparent;"));
        _serchMusic->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:23px;color:rgba(133,133,133,100); background-color:transparent;border:1px solid rgba(133,133,133,100)"));
        _slider1->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:rgba(133,133,133,100); height:4px;}QSlider::groove:Horizontal{background:rgba(133,133,133,50);height:6px;}QSlider::handle:Horizontal{background:rgba(133,133,133,100);height: 28px;width:28px;border-radius:14px;margin: -12 0px;}");
        _slider2->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:rgba(133,133,133,100); height:4px;}QSlider::groove:Horizontal{background:rgba(133,133,133,50);height:6px;}QSlider::handle:Horizontal{background:rgba(133,133,133,100);height: 28px;width:28px;border-radius:14px;margin: -12 0px;}");
        _slider3->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:rgba(133,133,133,100); height:4px;}QSlider::groove:Horizontal{background:rgba(133,133,133,50);height:6px;}QSlider::handle:Horizontal{background:rgba(133,133,133,100);height: 28px;width:28px;border-radius:14px;margin: -12 0px;}");
        _label4->setStyleSheet(QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:rgba(133,133,133,100); background-color:transparent;"));
        _label5->setStyleSheet(QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:rgba(133,133,133,100); background-color:transparent;"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(133,133,133,50)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(133,133,133,50)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(133,133,133,50)}"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{background-color:transparent} QPushButton:hover{background-color:rgba(133,133,133,50)}"));
        break;

    case 3:
        _menu->setStyleSheet("font-family:'华文中宋';color:white;font:bold;font-size:20px;background:rgb(75,251,75);selection-color: rgb(75,200,75);");
        _label1->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:23px;color:rgb(75,251,75); background:transparent;"));
        _serchMusic->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:23px;color:rgb(75,251,75); background-color:transparent;border:1px solid rgb(75,251,75)"));
        _slider1->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:rgb(75,251,75); height:4px;}QSlider::groove:Horizontal{background:rgba(133,133,133,50);height:6px;}QSlider::handle:Horizontal{background:rgb(75,251,75);height: 28px;width:28px;border-radius:14px;margin: -12 0px;}");
        _slider2->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:rgb(75,251,75); height:4px;}QSlider::groove:Horizontal{background:rgba(133,133,133,50);height:6px;}QSlider::handle:Horizontal{background:rgb(75,251,75);height: 28px;width:28px;border-radius:14px;margin: -12 0px;}");
        _slider3->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:rgb(75,251,75); height:4px;}QSlider::groove:Horizontal{background:rgba(133,133,133,50);height:6px;}QSlider::handle:Horizontal{background:rgb(75,251,75);height: 28px;width:28px;border-radius:14px;margin: -12 0px;}");
        _label4->setStyleSheet(QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:rgb(75,251,75); background-color:transparent;"));
        _label5->setStyleSheet(QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:rgb(75,251,75); background-color:transparent;"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{border-radius:20px;background-color:transparent} QPushButton:hover{background-color:rgba(75,251,75,50)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{border-radius:20px;background-color:transparent} QPushButton:hover{background-color:rgba(75,251,75,50)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{border-radius:20px;background-color:transparent} QPushButton:hover{background-color:rgba(75,251,75,50)}"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{border-radius:20px;background-color:transparent} QPushButton:hover{background-color:rgba(75,251,75,50)}"));
        break;

    case 4:
        _menu->setStyleSheet("font-family:'华文中宋';color:black;font:bold;font-size:20px;background:white;selection-color: red;");
        _label1->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:23px;color:black; background:transparent;"));
        _serchMusic->setStyleSheet(QStringLiteral("font-family:'华文中宋';font:bold;font-size:23px;color:black; background-color:transparent;border:1px solid black"));
        _slider1->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:black; height:4px;}QSlider::groove:Horizontal{background:rgba(133,133,133,50);height:6px;}QSlider::handle:Horizontal{background:black;height: 28px;width:28px;border-radius:14px;margin: -12 0px;}");
        _slider2->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:black; height:4px;}QSlider::groove:Horizontal{background:rgba(133,133,133,50);height:6px;}QSlider::handle:Horizontal{background:black;height: 28px;width:28px;border-radius:14px;margin: -12 0px;}");
        _slider3->setStyleSheet("QSlider::add-page:Horizontal{background-color:transparent; height:4px;}QSlider::sub-page:Horizontal{background-color:black; height:4px;}QSlider::groove:Horizontal{background:rgba(133,133,133,50);height:6px;}QSlider::handle:Horizontal{background:black;height: 28px;width:28px;border-radius:14px;margin: -12 0px;}");
        _slider4->setStyleSheet("QSlider::add-page:Vertical{background-color:transparent; width:6px;}QSlider::sub-page:Vertical{background-color:black; width:6px;}QSlider::groove:Vertical{background:transparent;width:6px;}QSlider::handle:Vertical{background:red;height: 12px;width:12px;margin:0px -4px;}");
        _label4->setStyleSheet(QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:black; background-color:transparent;"));
        _label4->setStyleSheet(QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:black; background-color:transparent;"));
        _label5->setStyleSheet(QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:black; background-color:transparent;"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{border-radius:20px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{border-radius:20px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{border-radius:20px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:30px;color:black;} QPushButton{border-radius:20px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button5->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:25px;font:bold;color:black;} QPushButton{border-radius:25px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button6->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:25px;font:bold;color:black;} QPushButton{border-radius:25px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button7->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:25px;font:bold;color:black;} QPushButton{border-radius:25px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button8->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:25px;color:black;} QPushButton{border-radius:25px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button9->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:25px;font:bold;color:black;} QPushButton{border-radius:25px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button10->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:25px;font:bold;color:black;} QPushButton{border-radius:25px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        _button11->setStyleSheet(QStringLiteral("QPushButton{font-family:'华文中宋';font-size:25px;font:bold;color:black;} QPushButton{border-radius:25px;background-color:transparent} QPushButton:hover{background-color:rgba(200,200,200,150)}"));
        break;

    default:break;
    }

    //控件位置
    switch (nowStyleKind) {
    case 0:
    case 1:
    case 2:
        _label1->setGeometry(699,90,262,130);
        _serchMusic->setGeometry(700,220,260,40);
        _slider1->setGeometry(700,300,260,30);
        _slider2->setGeometry(700,360,260,30);
        _label4->setGeometry(700,390,65,30);
        _label5->setGeometry(903,390,65,30);
        _slider3->setGeometry(700,420,260,30);
        _button1->setGeometry(795,463,70,50);
        _button2->setGeometry(715,523,70,50);
        _button3->setGeometry(875,523,70,50);
        _button4->setGeometry(795,583,70,50);
        _button5->setGeometry(150,80,400,50);
        _button6->setGeometry(100,145,500,60);
        _button7->setGeometry(60,220,580,70);
        _button8->setGeometry(30,305,640,80);
        _button9->setGeometry(60,400,580,70);
        _button10->setGeometry(100,485,500,60);
        _button11->setGeometry(150,560,400,50);
        break;
    case 3:
        _label1->setGeometry(585,90,262,130);
        _serchMusic->setGeometry(586,220,260,40);
        _slider1->setGeometry(586,300,260,30);
        _slider2->setGeometry(586,360,260,30);
        _label4->setGeometry(586,390,65,30);
        _label5->setGeometry(789,390,65,30);
        _slider3->setGeometry(586,420,260,30);
        _button1->setGeometry(681,463,70,50);
        _button2->setGeometry(601,523,70,50);
        _button3->setGeometry(761,523,70,50);
        _button4->setGeometry(681,583,70,50);
        break;
    case 4:
        _label1->setGeometry(699,90,262,130);
        _serchMusic->setGeometry(700,220,260,40);
        _slider1->setGeometry(700,300,260,30);
        _slider2->setGeometry(700,360,260,30);
        _label4->setGeometry(700,390,65,30);
        _label5->setGeometry(903,390,65,30);
        _slider3->setGeometry(700,420,260,30);
        _slider4->setGeometry(668,95,30,540);
        _button1->setGeometry(795,463,70,50);
        _button2->setGeometry(715,523,70,50);
        _button3->setGeometry(875,523,70,50);
        _button4->setGeometry(795,583,70,50);
        _button5->setGeometry(30,120,640,50);
        _button6->setGeometry(30,190,640,50);
        _button7->setGeometry(30,260,640,50);
        _button8->setGeometry(30,330,640,50);
        _button9->setGeometry(30,400,640,50);
        _button10->setGeometry(30,470,640,50);
        _button11->setGeometry(30,540,640,50);
        break;
    default:break;
    }

    _button5->setVisible(false);
    _button6->setVisible(false);
    _button7->setVisible(false);
    _button8->setVisible(false);
    _button9->setVisible(false);
    _button10->setVisible(false);
    _button11->setVisible(false);
    //对于样式0和4，切换样式后即显示歌曲列表
    if(nowStyleKind==0||nowStyleKind==4)ShowList();
    //只有样式4显示歌曲列表滚动条
    if(nowStyleKind==4)_slider4->setVisible(true);
    else _slider4->setVisible(false);
}

MusicWidget::~MusicWidget()
{
    delete timerGetTime;
    delete timerDelayGetTime;
    delete rePaintTimer;
    delete musicNameCptr;
    delete this;
}
