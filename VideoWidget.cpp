#include "VideoWidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QTimer>
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QMimeData>
#include <QScreen>
//#include <QVideoWidget>
//#include <QMediaPlayer>
//#include <QMediaPlaylist>


VideoWidget::VideoWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlag(Qt::FramelessWindowHint);//去除窗口标题栏
    QScreen* screen = QGuiApplication::primaryScreen();
    //根据屏幕分辨率设置窗口大小
    if(screen->availableGeometry().width()<1920)
        this->resize(1000,660);//窗口初始大小
    else
        this->resize(1400,850);//窗口初始大小

    //this->setAttribute(Qt::WA_TranslucentBackground,true);//实现窗口透明
    this->setWindowIcon(QIcon(":/I/Resources/NKPlayerICON2.ico"));
    this->setAcceptDrops(true);

    QFile file0("Setting/mp");
    file0.open(QIODevice::ReadOnly);
    file0.seek(0);
    mplayerDir = file0.readAll();
    file0.close();

    lay = new QVBoxLayout(this);
    lay1 = new QHBoxLayout(this);
    lay2 = new QHBoxLayout(this);
    lay3 = new QHBoxLayout(this);//标题栏
    lay->setSpacing(3);//设置控件间隔
    lay->setContentsMargins(0,0,0,0);//设置边框余量
    lay1->setContentsMargins(3,0,3,0);//设置边框余量
    lay2->setContentsMargins(20,0,20,3);//设置边框余量
    lay3->setContentsMargins(0,0,5,0);//设置边框余量
    lay->addLayout(lay3);
    lay->addWidget(&_videoShowWidget);//视频
    lay->addLayout(lay1);//进度条
    lay->addLayout(lay2);//功能按钮

    //lay1放当前时间，进度条，总时间
    lay1->addWidget(_label4=new QLabel(this));
    lay1->addWidget(_slider1 = new QSlider(Qt::Horizontal,this));
    lay1->addWidget(_label5=new QLabel(this));
    _label4->setMaximumHeight(20);//设置label的最大高度
    _label5->setMaximumHeight(20);//否则会导致lay1过高

    _slider1->setMinimum(0);
    _slider1->setMaximum(10000);
    _slider1->setFocusPolicy(Qt::NoFocus);

    lay2->addWidget(_button0=new QPushButton(this));//QStringLiteral("播放|暂停"),
    _button0->setFocusPolicy(Qt::NoFocus);
    lay2->addWidget(_button2=new QPushButton(this));//QStringLiteral("打开文件"),
    //将所有button都设置为NoFocus，避免button响应键盘消息影响使用键盘快捷键
    _button2->setFocusPolicy(Qt::NoFocus);
    lay2->addWidget(_button3=new QPushButton(this));//QStringLiteral("打开字幕"),
    _button3->setFocusPolicy(Qt::NoFocus);
    lay2->addWidget(_button9=new QPushButton(this));//截屏
    _button9->setFocusPolicy(Qt::NoFocus);


    lay2->addWidget(_label1=new QLabel(this));//QStringLiteral("音量")
    _label1->setToolTip(QStringLiteral("音量"));
    //音量滑杆
    lay2->addWidget(_slider2=new QSlider(Qt::Horizontal,this));
    _slider2->setFocusPolicy(Qt::NoFocus);
    _slider2->setMinimum(0);
    _slider2->setMaximum(100);
    _slider2->setValue(80);

    //播放器标志
    lay2->addWidget(_label3=new QLabel(this));

    //播放速度滑杆
    lay2->addWidget(_slider3=new QSlider(Qt::Horizontal,this));
    _slider3->setFocusPolicy(Qt::NoFocus);
    _slider3->setMinimum(5);
    _slider3->setMaximum(30);
    _slider3->setValue(10);

    lay2->addWidget(_label2=new QLabel(this));//QStringLiteral("速度"),
    _label2->setToolTip(QStringLiteral("播放速度"));
    lay2->addWidget(_button10=new QPushButton(this));//皮肤
    _button10->setFocusPolicy(Qt::NoFocus);
    lay2->addWidget(_button8=new QPushButton(this));//QStringLiteral("字幕提前"),
    _button8->setFocusPolicy(Qt::NoFocus);
    lay2->addWidget(_button7=new QPushButton(this));//QStringLiteral("字幕滞后"),
    _button7->setFocusPolicy(Qt::NoFocus);
    lay2->addWidget(_button1=new QPushButton(this));//QStringLiteral("字幕乱码"),
    _button1->setFocusPolicy(Qt::NoFocus);

    //标题栏
    lay3->addWidget(_label7=new QLabel(this));
    _label7->setPixmap(QPixmap(":/V/Resources/NKPlayerICON3.png"));
    lay3->addWidget(_label6=new QLabel(this));
    _label7->setMaximumHeight(20);
    _label6->setMaximumHeight(20);//设置label的最大高度
    lay3->addStretch();
    lay3->addWidget(_button4=new QPushButton(this));//QStringLiteral("——"),
    _button4->setFocusPolicy(Qt::NoFocus);
    lay3->addWidget(_button5=new QPushButton(this));//QStringLiteral("口"),
    _button5->setFocusPolicy(Qt::NoFocus);
    lay3->addWidget(_button6=new QPushButton(this));//QStringLiteral("X"),
    _button6->setFocusPolicy(Qt::NoFocus);
    _button4->setMaximumSize(20,20);
    _button5->setMaximumSize(20,20);
    _button6->setMaximumSize(20,20);
    lay3->setSpacing(10);
    _button4->setToolTip(QStringLiteral("最小化"));
    _button5->setToolTip(QStringLiteral("全屏"));
    _button6->setToolTip(QStringLiteral("关闭"));

    _button0->setMaximumSize(35,35);
    _button1->setMaximumSize(35,35);
    _button2->setMaximumSize(35,35);
    _button3->setMaximumSize(35,35);
    _button7->setMaximumSize(35,35);
    _button8->setMaximumSize(35,35);
    _button9->setMaximumSize(35,35);
    _button10->setMaximumSize(35,35);
    lay2->setSpacing(20);
    _button0->setToolTip(QStringLiteral("播放/暂停"));
    _button1->setToolTip(QStringLiteral("字幕乱码"));
    _button2->setToolTip(QStringLiteral("打开文件"));
    _button3->setToolTip(QStringLiteral("打开字幕"));
    _button7->setToolTip(QStringLiteral("字幕滞后"));
    _button8->setToolTip(QStringLiteral("字幕提前"));
    _button9->setToolTip(QStringLiteral("截屏"));
    _button10->setToolTip(QStringLiteral("切换皮肤"));

    //视频列表
    _button11 = new QPushButton(this);
    _button11->setFocusPolicy(Qt::NoFocus);
    _button12 = new QPushButton(this);
    _button12->setFocusPolicy(Qt::NoFocus);
    _button13 = new QPushButton(this);
    _button13->setFocusPolicy(Qt::NoFocus);
    _button14 = new QPushButton(this);
    _button14->setFocusPolicy(Qt::NoFocus);
    _button15 = new QPushButton(this);
    _button15->setFocusPolicy(Qt::NoFocus);
    _button16 = new QPushButton(this);
    _button16->setFocusPolicy(Qt::NoFocus);
    _button17 = new QPushButton(this);
    _button17->setFocusPolicy(Qt::NoFocus);
    _button11->setGeometry(0,65,this->width(),50);
    _button12->setGeometry(0,165,this->width(),50);
    _button13->setGeometry(0,265,this->width(),50);
    _button14->setGeometry(0,365,this->width(),50);
    _button15->setGeometry(0,465,this->width(),50);
    _button16->setGeometry(0,565,this->width(),50);
    _button17->setGeometry(0,665,this->width(),50);
    _label6->setVisible(false);
    _button11->setVisible(false);
    _button12->setVisible(false);
    _button13->setVisible(false);
    _button14->setVisible(false);
    _button15->setVisible(false);
    _button16->setVisible(false);
    _button17->setVisible(false);

    connect(_slider1, SIGNAL(sliderReleased()), this , SLOT(slotOnSlider1Released()));
    connect(_slider1, SIGNAL(sliderPressed()), this , SLOT(slotOnslider1Pressed()));
    connect(_slider1, SIGNAL(valueChanged(int)), this , SLOT(slotOnslider1ValueChanged(int)));
    connect(_slider2, SIGNAL(valueChanged(int)), this , SLOT(slotOnSlider2Changed(int)));
    connect(_slider3, SIGNAL(valueChanged(int)), this , SLOT(slotOnSlider3Changed(int)));
    connect(_button0, SIGNAL(clicked()), this , SLOT(slotButton0Clicked()));
    connect(_button1, SIGNAL(clicked()), this , SLOT(slotButton1Clicked()));
    connect(_button2, SIGNAL(clicked()), this , SLOT(slotButton2Clicked()));
    connect(_button3, SIGNAL(clicked()), this , SLOT(slotButton3Clicked()));
    connect(_button4, SIGNAL(clicked()), this , SLOT(slotButton4Clicked()));
    connect(_button5, SIGNAL(clicked()), this , SLOT(slotButton5Clicked()));
    connect(_button6, SIGNAL(clicked()), this , SLOT(slotButton6Clicked()));
    connect(_button7, SIGNAL(clicked()), this , SLOT(slotButton7Clicked()));
    connect(_button8, SIGNAL(clicked()), this , SLOT(slotButton8Clicked()));
    connect(_button9, SIGNAL(clicked()), this , SLOT(slotButton9Clicked()));
    connect(_button10, SIGNAL(clicked()), this , SLOT(slotButton10Clicked()));
    connect(_button11, SIGNAL(clicked()), this, SLOT(slotButton11_17Clicked()));
    connect(_button12, SIGNAL(clicked()), this, SLOT(slotButton11_17Clicked()));
    connect(_button13, SIGNAL(clicked()), this, SLOT(slotButton11_17Clicked()));
    connect(_button14, SIGNAL(clicked()), this, SLOT(slotButton11_17Clicked()));
    connect(_button15, SIGNAL(clicked()), this, SLOT(slotButton11_17Clicked()));
    connect(_button16, SIGNAL(clicked()), this, SLOT(slotButton11_17Clicked()));
    connect(_button17, SIGNAL(clicked()), this, SLOT(slotButton11_17Clicked()));

    //每隔一秒，查询一次视频当前播放的时间
    timerGetTime = new QTimer;
    connect(timerGetTime, SIGNAL(timeout()), this, SLOT(slotGetTime()));
    timerGetTime->setInterval(100);
    timerGetTime->start();

    //延迟500ms后再开始查询时间，避免手动调整滑块位置后，播放时间没能及时更新
    timerDelayGetTime = new QTimer;
    connect(timerDelayGetTime, SIGNAL(timeout()), this, SLOT(slotTimerDelayGetTimeOut()));
    timerDelayGetTime->setInterval(500);
    timerDelayGetTime->setSingleShot(true);

    //滚轮停止转动后延迟2秒，隐藏列表
    timerDelayHideList = new QTimer;
    connect(timerDelayHideList, SIGNAL(timeout()), this, SLOT(slotTimerDelayHideListOut()));
    timerDelayHideList->setInterval(2000);
    timerDelayHideList->setSingleShot(true);


    ReadSetting();//读取设置项
    SetAllStyle(nowStyleKind);
    slotButton2Clicked();//打开文件
}

//读取配置文件
void VideoWidget::ReadSetting()
{
    QFile file("Setting/VSetting");//指定文件
    file.open(QIODevice::ReadOnly);
    //读取上次设置的样式
    file.seek(1);//定位到第1个字节
    nowStyleKind = file.read(1).toInt();
    //读取上次打开文件的目录
    file.seek(3);
    _strDir = file.readLine();
    _strDir.resize(_strDir.size()-1);//去掉结尾的换行符
    file.close();
}


void VideoWidget::slotGetTime()
{
    //如果视频进程未启动，退出
    if(!VideoProcess.isOpen())return;
    //当视频暂停时不查询时间，否则视频会自动播放
    //当通过鼠标拖动滑块、左右方向键改变播放进度时，不查询时间，防止时间设置冲突
    //### 这里也可以通过在以下几个bool变量变true的位置调用timerGetTime->stop()，
    //### 变true的位置调用timerGetTime->start()来实现
    if(isPause||slider1_isPressed||key_RL_isPressed) return;

    VideoProcess.write(QString("get_time_length\n").toUtf8());//获取总时间
    VideoProcess.write(QString("get_time_pos\n").toUtf8());//获取当前时间
    QByteArray gettime = VideoProcess.readAll();//读取结果

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
            _slider1->setRange(0,secAll);
            _slider1->setValue(secNow);
        }
    }
}
void VideoWidget::slotTimerDelayGetTimeOut()
{
    canReadTimeValue = true;//开始读取查询结果中的时间
}

void VideoWidget::slotOnSlider1Released()
{
    slider1_isPressed = false;//鼠标释放滑块
    //seek的最后一个参数，模式为1，表示寻找到视频value%的地方开始播放，模式为2，表示寻找到value秒处开始播放

    //当鼠标拖动滑块改变进度完成鼠标释放时，将视频的进度设置为当前滑块位置所表示的值
    VideoProcess.write(QString("seek %1 2\n").arg(_slider1->value()).toUtf8());
    //延迟500ms后再开始读取查询结果中的时间
    timerDelayGetTime->start();

    //如果调整播放进度时，视频为暂停状态，则将其切换为播放状态
    //否则当进度调整完毕时，视频自动变为播放状态，但此时记录的仍然是暂停状态，导致时间不更新
    if(isPause){
        VideoProcess.write(QString("pause\n").toUtf8());
        isPause = 1-isPause;//暂停\播放状态切换
        SetAllStyle(nowStyleKind);
    }
}
void VideoWidget::slotOnslider1Pressed()
{
    slider1_isPressed = true;//鼠标按下滑块
    canReadTimeValue = false;//暂停读取查询结果中的时间
}
void VideoWidget::slotOnslider1ValueChanged(int value)
{
    //当鼠标拖动滑块使得滑块的值改变时，左侧的当前时间跟着变化
    if(slider1_isPressed){
        QString showNowTime;
        showNowTime += QString::number(value/60);
        showNowTime += ":";
        showNowTime += QString::number(value%60);
        _label4->setText(showNowTime);
    }
}


void VideoWidget::slotOnSlider2Changed(int value)
{
    //调整音量大小
    VideoProcess.write(QString("volume %1 1\n").arg(value).toUtf8());
}
void VideoWidget::slotOnSlider3Changed(int value)
{
    //调整播放速度
    double speed = double(value);
    speed /= 10;
    VideoProcess.write(QString("speed_set  %1 1\n").arg(speed).toUtf8());
}

//转换字幕编码
void VideoWidget::slotButton1Clicked()
{
    //若字幕路径为空，即没有打开字幕，则不响应
    if (_strSub.isEmpty())return;

    //打开配置文件
    QFile file("Setting/VSetting");
    file.open(QIODevice::ReadWrite);
    //定位到NotePad++路径开始处
    file.seek(300);
    //如果NotePad++路径为空
    if(file.read(1).isEmpty()){
        //获取NotePad++.exe路径
        _strNotePad = QFileDialog::getOpenFileName(
                    this,
                    QStringLiteral("打开 notepad++.exe"),
                    "/",
                    "(*.exe)");
        //未找到路径，不响应
        if (_strNotePad.isEmpty()) return;
        //找到路径后，将其写入配置文件相应位置
        file.seek(300);
        QString strNotePad;
        strNotePad += _strNotePad;
        strNotePad += "\n";
        file.write(strNotePad.toUtf8());
    }
    //若配置文件中的NotePad++路径不为空，则读取路径
    file.seek(300);
    _strNotePad = file.readLine();
    _strNotePad.resize(_strNotePad.size()-1);//去掉最后的换行符
    //qDebug() << _strNotePad;
    file.close();

    //在新进程中打开Notepad++
    QStringList argv;
    argv += _strSub;
    //用Notepad++打开此字幕
    SubtransProcess.start(_strNotePad , argv);
    //当进程结束，即字幕编码转换完成时重新加载此字幕
    connect(&SubtransProcess,SIGNAL(finished(int)), this ,SLOT(slotSubTransformed(int)));
}
void VideoWidget::slotSubTransformed(int)
{
    //重新加载字幕
    //构造加载字幕命令："sub_load directory\n"
    QString sub;
    sub += "sub_load ";
    sub += _strSub;
    sub += "\n";
    VideoProcess.write(sub.toUtf8());//加载字幕
    VideoProcess.write(QString("sub_file 1\n").toUtf8());//显示字幕
}

//播放暂停
void VideoWidget::slotButton0Clicked()
{
    VideoProcess.write(QString("pause\n").toUtf8());//暂停\播放
    isPause = 1-isPause;//暂停\播放状态切换
    SetAllStyle(nowStyleKind);
}

//打开文件
void VideoWidget::slotButton2Clicked()
{
    QString directory;
    //判断是否是拖入文件或命令行输入文件，若不是则使用模态对话框打开文件
    if(dropFilePath.isEmpty()&&filePathArg.isEmpty()){
        directory = QFileDialog::getOpenFileName(
                    this,
                    QStringLiteral("打开视频文件"),
                    _strDir,
                    "All files(*.*);;(*.mp4);;(*.mkv);;(*.avi);;(*.rmvb);;(*.flv);;(*.gif)");
    }
    //若拖入文件路径不为空，则使用拖入文件路径，并将拖入文件路径清空
    else if(!dropFilePath.isEmpty()){
        directory = dropFilePath;
        dropFilePath.clear();
    }
    //若命令行输入路径不为空，则使用命令行文件路径，并将命令行文件路径清空
    else if(!filePathArg.isEmpty()){
        directory = filePathArg;
        filePathArg.clear();
    }

    //如果没有选择任何文件则返回
    if (directory.isEmpty()) return;
    else{
        QFileInfo fileInfo(directory);
        //如果打开的文件不是下列类型则返回
        if(!(fileInfo.suffix()=="mp4"||fileInfo.suffix()=="mkv"||fileInfo.suffix()=="avi"||
             fileInfo.suffix()=="rmvb"||fileInfo.suffix()=="flv"||fileInfo.suffix()=="gif")){
            QMessageBox::critical(this, QStringLiteral("打开错误"), QStringLiteral("暂不支持此类型，请重新选择文件！"));
            return;
        }
        _strDir = fileInfo.path();//保存这次打开的文件所在文件夹的路径
        //否则如果有正在播放的视频进程，则将其关闭
        if(VideoProcess.isOpen()){
            VideoProcess.write(QString("quit\n").toUtf8());
            VideoProcess.close();
        }
    }
    videoPathList.clear();
    videoNameList.clear();
    listShowMid=0;
    //获取同文件夹下的视频列表
    QDir dir(_strDir);
    dir.setFilter(QDir::Files);//设置过滤器：只筛选文件，不筛选文件夹
    QFileInfoList fileInfoList = dir.entryInfoList();//获取文件列表
    for(int i=0; i<fileInfoList.count(); i++){
        //筛选文件后缀
        if(fileInfoList.at(i).suffix()=="mp4"||fileInfoList.at(i).suffix()=="mkv"||fileInfoList.at(i).suffix()=="avi"||
                fileInfoList.at(i).suffix()=="rmvb"||fileInfoList.at(i).suffix()=="flv"||fileInfoList.at(i).suffix()=="gif"){
            videoPathList << fileInfoList.at(i).filePath();//文件完整路径
            videoNameList << fileInfoList.at(i).fileName();//文件名加后缀
        }
    }
    //qDebug()<<videoPathList.size();

    //获取当前播放视频在整个列表中的位置
    for(int i=0;i<videoPathList.count();i++){
        if(videoPathList.at(i)==directory) nowPlayInList = i;
    }

    QStringList args;
    args << "-slave";//使用管道命令
    args << "-quiet";
    args << "-zoom";
    args << directory;
    //获取当前窗口号，并将mplayer的输出嵌入当前窗口中
    args << "-wid" << QString::number(_videoShowWidget.winId());//将_videoShowWidget的窗口号转换为字符串添加在-wid后面

    VideoProcess.start(mplayerDir, args);
    VideoProcess.write(QString("vo_fullscreen\n").toUtf8());//_videoShowWidget窗口全屏
}

void VideoWidget::dragEnterEvent(QDragEnterEvent* event)
{
    qDebug()<<"drop enter";
    //取出文件路径，用compare筛选文件后缀，compare相等时返回0，不是指定类型的文件则不响应
    if(!event->mimeData()->urls()[0].fileName().right(3).compare("mp4")||
            !event->mimeData()->urls()[0].fileName().right(3).compare("mkv")||
            !event->mimeData()->urls()[0].fileName().right(3).compare("avi")||
            !event->mimeData()->urls()[0].fileName().right(4).compare("rmvb")||
            !event->mimeData()->urls()[0].fileName().right(3).compare("flv")||
            !event->mimeData()->urls()[0].fileName().right(3).compare("gif")){
        event->acceptProposedAction();
        //取出文件路径
        dropFilePath = event->mimeData()->urls().first().toLocalFile();
        //调用打开文件
        slotButton2Clicked();
    }
    else event->ignore();
}
void VideoWidget::dropEvnet(QDropEvent* )
{
    //这里应该是因为主窗口上有show窗口覆盖的原因，只能检测到dragEnterEvent，检测不到dropEvent
    //否则取出拖入文件路径的操作应该放在这里
    qDebug()<<"drop file";
}

void VideoWidget::slotButton3Clicked()
{
    //打开字幕
    _strSub = QFileDialog::getOpenFileName(
                this,
                QStringLiteral("打开字幕文件"),
                _strDir,
                "All files(*.*);;(*.srt);;(*.ass);;(*.sub);;(*.ssa);;(*.smi)");
    //if (!_strSub.isEmpty())qDebug() << _strSub;
    QFileInfo fileInfo(_strSub);//获取文件目录
    _strDir = fileInfo.filePath();//保存这次打开文件目录

#if 0
    //尝试打开字幕文件，利用C++自身来转换字幕文件的编码
    //待开发。。。。。。
    QFile file(_strDir);
    file.open(QIODevice::ReadWrite);
    QString subs = file.readAll();
    file.seek(0);
    file.write(subs.toStdWString());
    file.close();
#endif

    //构造加载字幕命令："sub_load directory\n"
    QString sub;
    sub += "sub_load ";
    sub += _strSub;
    sub += "\n";
    VideoProcess.write(sub.toUtf8());//加载字幕
    VideoProcess.write(QString("sub_file 1\n").toUtf8());//显示字幕
}

void VideoWidget::slotButton4Clicked()
{
    this->showMinimized();//最小化
}
void VideoWidget::slotButton5Clicked()
{
    //窗口全屏\活动切换
    if(this->windowState()==Qt::WindowFullScreen){
        this->setWindowState(Qt::WindowActive);
    }
    else this->setWindowState(Qt::WindowFullScreen);
}
void VideoWidget::slotButton6Clicked()
{
    this->close();//退出
    //exit(1);
}
void VideoWidget::slotButton7Clicked()
{
    VideoProcess.write(QString("sub_delay 0.3\n").toUtf8());//字幕滞后
}
void VideoWidget::slotButton8Clicked()
{
    VideoProcess.write(QString("sub_delay -0.3\n").toUtf8());//字幕提前
}
void VideoWidget::slotButton9Clicked()//截屏
{
    Screenshot();
}
void VideoWidget::slotButton10Clicked()//切换皮肤
{
    //nowStyleKind=5时回滚到0
    if(nowStyleKind < 8)nowStyleKind = (nowStyleKind+1)%8;
    SetAllStyle(nowStyleKind);
}

void VideoWidget::keyPressEvent(QKeyEvent * ev)
{
    //ev->modifiers();//修饰符，使用组合键时要使用
    int key = ev->key();//获取的都是大写字母的ascii码
    //如果按下Esc键
    if(key==16777216){
        //如果是全屏状态，则回到活动状态
        if(this->windowState()==Qt::WindowFullScreen){
            this->setWindowState(Qt::WindowActive);
            hideORshowToolsbar();
        }
        else this->close();//如果是活动状态则退出
    }
    //如果按下Enter键
    if(key==16777220){
        if(this->windowState()==Qt::WindowFullScreen){
            this->setWindowState(Qt::WindowActive);
        }
        else this->setWindowState(Qt::WindowFullScreen);
        hideORshowToolsbar();
    }
    //空格键暂停\播放
    if(key==32){
        VideoProcess.write(QString("pause\n").toUtf8());//暂停\播放
        isPause = 1-isPause;//暂停\播放状态切换
        SetAllStyle(nowStyleKind);
    }
    //'<'字幕滞后
    if(key==44){
         VideoProcess.write(QString("sub_delay 0.3\n").toUtf8());//字幕滞后
    }
    //'>'字幕提前
    if(key==46){
         VideoProcess.write(QString("sub_delay -0.3\n").toUtf8());//字幕提前
    }
    //F打开文件
    if(key==70) slotButton2Clicked();
    //L打开字幕
    if(key==76) slotButton3Clicked();

    //Left后退
    if(key==16777234){
        key_RL_isPressed = true;
        secNow-=15;
        _slider1->setValue(secNow);
        //VideoProcess.write(QString("seek %1 2\n").arg(secNow).toUtf8());
    }
    //Right前进
    if(key==16777236){
        key_RL_isPressed = true;
        secNow+=15;
        _slider1->setValue(secNow);
        //VideoProcess.write(QString("seek %1 2\n").arg(secNow).toUtf8());
    }
    //当左右方向键按下时，左侧当前时间跟着变化
    if(key_RL_isPressed){
        //如果调整播放进度时，视频为暂停状态，则将其切换为播放状态
        //否则当进度调整完毕时，视频自动变为播放状态，但此时记录的仍然是暂停状态，导致时间不更新
        if(isPause){
            VideoProcess.write(QString("pause\n").toUtf8());
            isPause = 1-isPause;//暂停\播放状态切换
            SetAllStyle(nowStyleKind);
        }
        canReadTimeValue = false;//暂停读取查询结果中的时间
        QString showNowTime;
        showNowTime += QString::number(secNow/60);
        showNowTime += ":";
        showNowTime += QString::number(secNow%60);
        _label4->setText(showNowTime);

        //当在全屏状态下通过左右方向键调整进度时，显示进度条和两个时间label
        if(this->windowState()==Qt::WindowFullScreen){
            _slider1->setVisible(true);
            _label4->setVisible(true);
            _label5->setVisible(true);
        }
    }
    //Up音量加
    if(key==16777235) _slider2->setValue(_slider2->value()+3);
    //Down音量减
    if(key==16777237) _slider2->setValue(_slider2->value()-3);

    //K字幕乱码，转换编码
    if(key==75) slotButton1Clicked();

    //J截屏
    if(key==74) Screenshot();

    //H切换样式
    if(key==72){
        //nowStyleKind=5时回滚到0
        if(nowStyleKind < 8)nowStyleKind = (nowStyleKind+1)%8;
        SetAllStyle(nowStyleKind);
    }

    //G手动改变NotePad++.exe的路径
    if(key==71){
        QFile file("Setting/VSetting");
        file.open(QIODevice::ReadWrite);
        _strNotePad = QFileDialog::getOpenFileName(
                    this,
                    QStringLiteral("打开notepad++.exe"),
                    "/",
                    "(*.exe)");
        //路径不为空
        if (!_strNotePad.isEmpty()){
            //找到路径后，将其写入配置文件相应位置
            file.seek(300);
            QString strNotePad;
            strNotePad += _strNotePad;
            strNotePad += "\n";
            file.write(strNotePad.toUtf8());
            file.close();
        }
    }

    //qDebug()<<key;
    return QWidget::keyPressEvent(ev);
}

void VideoWidget::hideORshowToolsbar()
{
    //当窗口处于活动状态且工具栏处于隐藏状态时，显示工具栏
    if(this->windowState()==Qt::WindowActive && true==toolsHide)
    {
        _slider1->setVisible(true);
        _slider2->setVisible(true);
        _slider3->setVisible(true);
        _button0->setVisible(true);
        _button1->setVisible(true);
        _button2->setVisible(true);
        _button3->setVisible(true);
        _button4->setVisible(true);
        _button5->setVisible(true);
        _button6->setVisible(true);
        _button7->setVisible(true);
        _button8->setVisible(true);
        _button9->setVisible(true);
        _button10->setVisible(true);
        _label1->setVisible(true);
        _label2->setVisible(true);
        _label3->setVisible(true);
        _label4->setVisible(true);
        _label5->setVisible(true);
        _label6->setVisible(true);
        _label7->setVisible(true);
        SetAllStyle(nowStyleKind);//重新回到原来的样式
        toolsHide = false;
    }
    //当窗口处于全屏状态且工具栏处于显示状态时，隐藏工具栏
    if(this->windowState()==Qt::WindowFullScreen && false==toolsHide)
    {
        _slider1->setVisible(false);
        _slider2->setVisible(false);
        _slider3->setVisible(false);
        _button0->setVisible(false);
        _button1->setVisible(false);
        _button2->setVisible(false);
        _button3->setVisible(false);
        _button4->setVisible(false);
        _button5->setVisible(false);
        _button6->setVisible(false);
        _button7->setVisible(false);
        _button8->setVisible(false);
        _button9->setVisible(false);
        _button10->setVisible(false);
        _label1->setVisible(false);
        _label2->setVisible(false);
        _label3->setVisible(false);
        _label4->setVisible(false);
        _label5->setVisible(false);
        _label6->setVisible(false);
        _label7->setVisible(false);
        this->setStyleSheet("background-color: black");
        toolsHide = true;
    }
}

//当一直按着某个键时，会不停的触发keyPressEvent和keyReleaseEvent
void VideoWidget::keyReleaseEvent(QKeyEvent * ev)
{
    int key = ev->key();
    //当释放左右方向键时
    if(key==16777234||key==16777236){
        //qDebug()<<secNow;

        key_RL_isPressed = false;
        //视频进度定位到改变后的秒数
        VideoProcess.write(QString("seek %1 2\n").arg(secNow).toUtf8());

        timerDelayGetTime->start();//延迟500ms后再开始读取查询结果中的时间

        //当在全屏状态下通过左右方向键调整进度完毕时，隐藏进度条和两个时间label
        if(this->windowState()==Qt::WindowFullScreen){
            _slider1->setVisible(false);
            _label4->setVisible(false);
            _label5->setVisible(false);
        }
    }

    return QWidget::keyReleaseEvent(ev);
}


///*去掉标题栏后，实现鼠标拖动窗口*/
void VideoWidget::mousePressEvent(QMouseEvent *event)
{
    //记录鼠标按压位置
    int x = event->pos().x();
    int y = event->pos().y();
    if (x >= 0 && x <= this->width() && event->button()==Qt::LeftButton &&
            ((y>=this->height()-100&&y<=this->height())||(y>0&&y<20)))
    {
        m_isPressed = true;//按压位置在窗口内
        m_startMovePos = event->pos();//将鼠标按下位置作为窗口移动的起始坐标
    }

    //鼠标右键点击
    //跳转到（点击位置横坐标/窗口宽度）*视频总长度 的位置播放
    if (event->button()==Qt::RightButton)
    {
        //如果调整播放进度时，视频为暂停状态，则将其切换为播放状态
        //否则当进度调整完毕时，视频自动变为播放状态，但此时记录的仍然是暂停状态，导致时间不更新
        if(isPause){
            VideoProcess.write(QString("pause\n").toUtf8());
            isPause = 1-isPause;//暂停\播放状态切换
            SetAllStyle(nowStyleKind);
        }
        VideoProcess.write(QString("seek %1 2\n").arg(x*secAll/this->width()).toUtf8());
    }
    //鼠标左键单击暂停\播放
    if (event->button()==Qt::LeftButton){
        //不是全屏状态在视频部分点击，是全屏状态任意位置点击
        if((x >= 0 && x <= this->width() && y < this->height()-100 && y > 20 && this->windowState()!=Qt::WindowFullScreen) ||
                this->windowState()==Qt::WindowFullScreen)
        VideoProcess.write(QString("pause\n").toUtf8());
        isPause = 1-isPause;//暂停\播放状态切换
        SetAllStyle(nowStyleKind);
    }
    //鼠标中键单击全屏\活动
    if (event->button()==Qt::MiddleButton){
        if(this->windowState()==Qt::WindowFullScreen){
            this->setWindowState(Qt::WindowActive);
        }
        else this->setWindowState(Qt::WindowFullScreen);
        hideORshowToolsbar();
    }

    return QWidget::mousePressEvent(event);
}
void VideoWidget::mouseMoveEvent(QMouseEvent *event)
{
    //当鼠标按在窗口内部并且是活动窗口时
    if (m_isPressed && this->windowState()!=Qt::WindowFullScreen)
    {
        QPoint movePoint = event->globalPos();//记录鼠标当前移动到的位置
        //m_startMovePos = event->globalPos();
        //窗口在x,y轴分量的的移动距离分别为movePoint和m_startMovePos坐标分量的差值
        this->move(movePoint.x() - m_startMovePos.x(), movePoint.y() - m_startMovePos.y());
    }

    return QWidget::mouseMoveEvent(event);
}
void VideoWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;//鼠标释放
    return QWidget::mouseReleaseEvent(event);
}


void VideoWidget::wheelEvent(QWheelEvent* event)
{
    //如果列表为空，则返回
    if(videoNameList.isEmpty()) return;
    //如果延迟隐藏计时器已经开始，则停止
    if(timerDelayHideList->isActive())timerDelayHideList->stop();
    _label6->setVisible(true);
    _button11->setVisible(true);
    _button12->setVisible(true);
    _button13->setVisible(true);
    _button14->setVisible(true);
    _button15->setVisible(true);
    _button16->setVisible(true);
    _button17->setVisible(true);

    //根据滚轮的滚动方向增加或减小列表序号保存到相应的buttonValue中，并且更新列表上的歌曲名称

    if(event->delta()<0)listShowMid++;
    if(event->delta()>0)listShowMid--;
    //如果listShowMid大于总数，则取余
    if(listShowMid>=videoNameList.count()) listShowMid %= videoNameList.count();
    //如果listShowMid小于零，则加上总数
    if(listShowMid<0)listShowMid= listShowMid+videoNameList.count();

    //比listShowMid小的要防止下溢
    _button11->setText(videoNameList.at( (listShowMid-3+videoNameList.count())%videoNameList.count() ).toUtf8());
    _button11Value =  (listShowMid-3+videoNameList.count())%videoNameList.count() ;
    _button12->setText(videoNameList.at( (listShowMid-2+videoNameList.count())%videoNameList.count() ).toUtf8());
    _button12Value =  (listShowMid-2+videoNameList.count())%videoNameList.count() ;
    _button13->setText(videoNameList.at( (listShowMid-1+videoNameList.count())%videoNameList.count() ).toUtf8());
    _button13Value =  (listShowMid-1+videoNameList.count())%videoNameList.count() ;
    //listShowMid放在中间
    _button14->setText(videoNameList.at(listShowMid).toUtf8());_button14Value = listShowMid;
    //比listShowMid大的要防止上溢
    _button15->setText(videoNameList.at((listShowMid+1)%videoNameList.count()).toUtf8());
    _button15Value = (listShowMid+1)%videoNameList.count();
    _button16->setText(videoNameList.at((listShowMid+2)%videoNameList.count()).toUtf8());
    _button16Value = (listShowMid+2)%videoNameList.count();
    _button17->setText(videoNameList.at((listShowMid+3)%videoNameList.count()).toUtf8());
    _button17Value = (listShowMid+3)%videoNameList.count();

    //在标题栏显示当前播放
    _label6->setText(QStringLiteral("正在播放:")+videoNameList.at(nowPlayInList));

    //延迟隐藏计时器开始
    timerDelayHideList->start();
    return QWidget::wheelEvent(event);
}
void VideoWidget::slotButton11_17Clicked()
{
    //手动调用，以实现点击列表后，列表立即隐藏
    slotTimerDelayHideListOut();
    //如果当前有播放进程将其关闭
    if(VideoProcess.isOpen())VideoProcess.close();
    //获取点击的按钮对应的文件路径,并且更新当前播放在列表中的位置
    QString videoPath;
    if(sender()==_button11){ videoPath=videoPathList.at(_button11Value);nowPlayInList=_button11Value;}
    if(sender()==_button12){ videoPath=videoPathList.at(_button12Value);nowPlayInList=_button12Value;}
    if(sender()==_button13){ videoPath=videoPathList.at(_button13Value);nowPlayInList=_button13Value;}
    if(sender()==_button14){ videoPath=videoPathList.at(_button14Value);nowPlayInList=_button14Value;}
    if(sender()==_button15){ videoPath=videoPathList.at(_button15Value);nowPlayInList=_button15Value;}
    if(sender()==_button16){ videoPath=videoPathList.at(_button16Value);nowPlayInList=_button16Value;}
    if(sender()==_button17){ videoPath=videoPathList.at(_button17Value);nowPlayInList=_button17Value;}
    //播放文件
    QStringList args;
    args << "-slave"<< "-quiet"<< "-zoom"<< videoPath<< "-wid" << QString::number(_videoShowWidget.winId());
    VideoProcess.start(mplayerDir, args);
    VideoProcess.write(QString("vo_fullscreen\n").toUtf8());//_videoShowWidget窗口全屏
}
void VideoWidget::slotTimerDelayHideListOut()
{
    _label6->setVisible(false);
    _button11->setVisible(false);
    _button12->setVisible(false);
    _button13->setVisible(false);
    _button14->setVisible(false);
    _button15->setVisible(false);
    _button16->setVisible(false);
    _button17->setVisible(false);
}

void VideoWidget::Screenshot()
{
    //截屏
    //保存截屏的文件夹为空
    if(_strScreenshot.isEmpty()){
        //暂停视频
        VideoProcess.write(QString("pause\n").toUtf8());
        isPause = 1-isPause;//暂停\播放状态切换
        SetAllStyle(nowStyleKind);
        //选择文件夹
        _strScreenshot = QFileDialog::getExistingDirectory(
                    this,
                    QStringLiteral("选择文件夹以保存截屏"),
                    _strDir,
                    QFileDialog::ShowDirsOnly);
        //如果选择的路径仍为空，则将视频所在文件夹作为截屏保存文件夹
        if(_strScreenshot.isEmpty()) _strScreenshot=_strDir;
        //选择文件夹的时候不截屏，因为此时截屏有文件夹选择对话框的残影
        return;
    }
    //mplayer自己的截屏命令不起作用
    //screenshot <value>
    //VideoProcess.write(QString("screenshot 0\n").toUtf8());

    //对整个屏幕截图
    QScreen* screen = QGuiApplication::primaryScreen();
    //在整个屏幕的截图中选取_videoShowWidget窗口所在的矩形框区域保存到pixmap
    //注意this窗口的位置是相对屏幕的，而_videoShowWidget窗口的位置是相对this的
    QPixmap screenshot = screen->grabWindow(0).copy(QRect(this->x()+_videoShowWidget.x(),
                                                          this->y()+_videoShowWidget.y(),
                                                          _videoShowWidget.width(),
                                                          _videoShowWidget.height()));
    //构造文件名
    QString filename = _strScreenshot;
    filename += "/";
    //用截图时间作为文件名
    filename += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");//
    filename += ".png";
    //将pixmap保存到文件
    screenshot.save(filename, nullptr , -1);
}

void VideoWidget::closeEvent(QCloseEvent *event)
{
    VideoProcess.kill();
    //关闭之前保存路径，样式等
    QFile file("Setting/VSetting");//指定文件
    file.open(QIODevice::ReadWrite);//读写模式打开指定的文件
    file.seek(1);//定位到第1个字节
    file.write(QByteArray::number(nowStyleKind));
    file.seek(2);
    file.write("\n");
    file.seek(3);
    QString strDir;
    strDir += _strDir;
    strDir += "\n";
    file.write(strDir.toUtf8());
    file.seek(299);
    file.write("\n");
    file.close();
    return QWidget::closeEvent(event);
}


//设置播放器样式
void VideoWidget::SetAllStyle(int styleKind)
{
    QString buttonStyle2;
    QString labelStyle1;
    QString sliderStyle1;
    QString sliderStyle2;

    switch (styleKind) {
    case 0:
        //窗口初始背景颜色
        this->setStyleSheet("background-color: white");
        if(isPause)_button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_play.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        else _button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_pause.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_videofile.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfile.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button8->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subdelay.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button7->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfront.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_suberror.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button9->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_screenshot.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button10->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_changestyle.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _label1->setPixmap(QPixmap(":/V/Resources/button_vol.png"));
        _label2->setPixmap(QPixmap(":/V/Resources/button_speed.png"));
        _label3->setPixmap(QPixmap(":/V/Resources/nkplayer1.png"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_mini.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button5->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_full.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button6->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_close.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));

        //列表button样式
        buttonStyle2=QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:27px;color:rgb(255,80,80);}QPushButton{background-color:transparent;}QPushButton:hover{background-color:rgb(200,200,200)}");
        //时间label样式
        labelStyle1=QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:rgb(255,80,80);background:transparent;");
        //音量、速度slider样式
        sliderStyle1="QSlider::add-page:Horizontal{background-color:rgb(200,250,200); height:4px;}QSlider::sub-page:Horizontal{background-color:yellow; height:4px;}QSlider::groove:Horizontal{background:transparent;height:6px;}QSlider::handle:Horizontal{height: 20px;width:20px;background-color:rgb(50,200,200);margin: -8 0px;}";
        //主进度条slider样式
        sliderStyle2="QSlider::add-page:Horizontal{background-color:rgb(200,250,200);height:4px;}QSlider::sub-page:Horizontal{background-color:yellow;height:4px;}QSlider::groove:Horizontal{background:transparent;height:6px;}QSlider::handle:Horizontal{height: 20px;width:20px;background-color:rgb(50,200,200);margin: -8 0px;}";
        break;

    case 1:
        //窗口初始背景颜色
        this->setStyleSheet("background-color: black");
        if(isPause)_button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_play.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        else _button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_pause.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_videofile.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfile.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button8->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subdelay.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button7->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfront.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_suberror.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button9->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_screenshot.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button10->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_changestyle.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _label1->setPixmap(QPixmap(":/V/Resources/button_vol.png"));
        _label2->setPixmap(QPixmap(":/V/Resources/button_speed.png"));
        _label3->setPixmap(QPixmap(":/V/Resources/nkplayer.png"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_mini.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button5->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_full.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button6->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_close.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));

        //列表button样式
        buttonStyle2=QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:27px;color:rgb(255,80,80);}QPushButton{background-color:transparent;}QPushButton:hover{background-color:rgb(100,100,100)}");
        //时间label样式
        labelStyle1=QStringLiteral("font-family:'微软雅黑';font:bold;font-size:20px;color:rgb(255,80,80);background:transparent;");
        //音量、速度slider样式
        sliderStyle1="QSlider::add-page:Horizontal{background-color:rgb(200,250,200); height:4px;} QSlider::sub-page:Horizontal{background-color:yellow; height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:rgb(50,200,200);margin: -8 0px;}";
        //主进度条slider样式
        sliderStyle2="QSlider::add-page:Horizontal{background-color:rgb(200,250,200);height:4px;} QSlider::sub-page:Horizontal{background-color:yellow;height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:rgb(50,200,200);margin: -8 0px;}";
        break;

    case 2:
        //窗口初始背景颜色
        this->setStyleSheet("background-color: black");
        if(isPause)_button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_play2.png);}QPushButton:hover{background-color:red}"));
        else _button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_pause2.png);}QPushButton:hover{background-color:red}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_videofile2.png);}QPushButton:hover{background-color:red}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfile2.png);}QPushButton:hover{background-color:red}"));
        _button8->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subdelay2.png);}QPushButton:hover{background-color:red}"));
        _button7->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfront2.png);}QPushButton:hover{background-color:red}"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_suberror2.png);}QPushButton:hover{background-color:red}"));
        _button9->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_screenshot2.png);}QPushButton:hover{background-color:red}"));
        _button10->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_changestyle2.png);}QPushButton:hover{background-color:red}"));
        _label1->setPixmap(QPixmap(":/V/Resources/button_vol2.png"));
        _label2->setPixmap(QPixmap(":/V/Resources/button_speed2.png"));
        _label3->setPixmap(QPixmap(":/V/Resources/nkplayer.png"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_mini2.png);}QPushButton:hover{background-color:red}"));
        _button5->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_full2.png);}QPushButton:hover{background-color:red}"));
        _button6->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_close2.png);}QPushButton:hover{background-color:red}"));

        //列表button样式
        buttonStyle2=QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:27px;color:rgb(100,230,150);}QPushButton{background-color:transparent;}QPushButton:hover{background-color:rgb(100,100,100)}");
        //时间label样式
        labelStyle1=QStringLiteral(" font-family:'微软雅黑';font:bold;font-size:20px;color:rgb(0,230,230); background:transparent;");
        //音量、速度slider样式
        sliderStyle1=" QSlider::add-page:Horizontal{background-color:rgb(0, 50, 50); height:4px;} QSlider::sub-page:Horizontal{background-color:rgb(0,230,230); height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:rgbrgb(0,230,230);margin: -8 0px;}";
        //主进度条slider样式
        sliderStyle2=" QSlider::add-page:Horizontal{background-color: rgb(0, 50, 50);height:4px;} QSlider::sub-page:Horizontal{background-color:rgbrgb(0,230,230);height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:rgbrgb(0,230,230);margin: -8 0px;}";
        break;

    case 3:
        //窗口初始背景颜色
        this->setStyleSheet("background-color: rgb(0, 100, 50)");
        if(isPause)_button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_play1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));
        else _button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_pause1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_videofile1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfile1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));
        _button8->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subdelay1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));
        _button7->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfront1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_suberror1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));
        _button9->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_screenshot1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));
        _button10->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_changestyle1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));
        _label1->setPixmap(QPixmap(":/V/Resources/button_vol1.png"));
        _label2->setPixmap(QPixmap(":/V/Resources/button_speed1.png"));
        _label3->setPixmap(QPixmap(":/V/Resources/nkplayer2.png"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_mini1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));
        _button5->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_full1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));
        _button6->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_close1.png);}QPushButton:hover{background-color:rgb(200,150,0)}"));

        //列表button样式
        buttonStyle2=QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:27px;color:rgb(200,150,0);}QPushButton{background-color:transparent;}QPushButton:hover{background-color:rgb(100,100,100)}");
        //时间label样式
        labelStyle1=QStringLiteral(" font-family:'微软雅黑';font:bold;font-size:20px;color:rgb(200,150,0); background:transparent;");
        //音量、速度slider样式
        sliderStyle1=" QSlider::add-page:Horizontal{background-color:rgb(140, 150, 160); height:4px;} QSlider::sub-page:Horizontal{background-color:black; height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;margin: -8 0px;}";
        //主进度条slider样式
        sliderStyle2=" QSlider::add-page:Horizontal{background-color: rgb(140, 150, 160);height:4px;} QSlider::sub-page:Horizontal{background-color:black;height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;margin: -8 0px;}";
        break;

    case 4:
        //窗口初始背景颜色
        this->setStyleSheet("background-color: yellow");
        if(isPause)_button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_play1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        else _button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_pause1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_videofile1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfile1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button8->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subdelay1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button7->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfront1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_suberror1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button9->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_screenshot1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button10->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_changestyle1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _label1->setPixmap(QPixmap(":/V/Resources/button_vol1.png"));
        _label2->setPixmap(QPixmap(":/V/Resources/button_speed1.png"));
        _label3->setPixmap(QPixmap(":/V/Resources/nkplayer2.png"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_mini1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button5->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_full1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));
        _button6->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_close1.png);}QPushButton:hover{background-color:rgb(150,250,100)}"));

        //列表button样式
        buttonStyle2=QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:27px;color:red;}QPushButton{background-color:transparent;}QPushButton:hover{background-color:rgb(200,200,200)}");
        //时间label样式
        labelStyle1=QStringLiteral(" font-family:'微软雅黑';font:bold;font-size:20px;color:black; background:transparent;");
        //音量、速度slider样式
        sliderStyle1="QSlider::add-page:Horizontal{background-color:rgb(140, 150, 160); height:4px;} QSlider::sub-page:Horizontal{background-color:black; height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;margin: -8 0px;}";
        //主进度条slider样式
        sliderStyle2=" QSlider::add-page:Horizontal{background-color: rgb(140, 150, 160);height:4px;} QSlider::sub-page:Horizontal{background-color:black;height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;margin: -8 0px;}";
        break;

    case 5:
        //窗口初始背景颜色
        this->setStyleSheet("background-color: red");
        if(isPause)_button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_play1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));
        else _button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_pause1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_videofile1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfile1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));
        _button8->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subdelay1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));
        _button7->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfront1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_suberror1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));
        _button9->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_screenshot1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));
        _button10->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_changestyle1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));
        _label1->setPixmap(QPixmap(":/V/Resources/button_vol1.png"));
        _label2->setPixmap(QPixmap(":/V/Resources/button_speed1.png"));
        _label3->setPixmap(QPixmap(":/V/Resources/nkplayer2.png"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_mini1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));
        _button5->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_full1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));
        _button6->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_close1.png);}QPushButton:hover{background-color:rgb(250,250,150)}"));

        //列表button样式
        buttonStyle2=QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:27px;color:white;}QPushButton{background-color:transparent;}QPushButton:hover{background-color:rgb(100,100,100)}");
        //时间label样式
        labelStyle1=QStringLiteral(" font-family:'微软雅黑';font:bold;font-size:20px;color:rgb(250,250,150); background:transparent;");
        //音量、速度slider样式
        sliderStyle1=" QSlider::add-page:Horizontal{background-color:rgb(250,250,150); height:4px;} QSlider::sub-page:Horizontal{background-color:black; height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;margin: -8 0px;}";
        //主进度条slider样式
        sliderStyle2=" QSlider::add-page:Horizontal{background-color: rgb(250,250,150);height:4px;} QSlider::sub-page:Horizontal{background-color:black;height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;margin: -8 0px;}";
        break;

    case 6:
        //窗口初始背景颜色
        this->setStyleSheet("background-color: rgb(100,200,250)");
        if(isPause)_button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_play1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        else _button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_pause1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_videofile1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfile1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button8->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subdelay1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button7->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfront1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_suberror1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button9->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_screenshot1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button10->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_changestyle1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _label1->setPixmap(QPixmap(":/V/Resources/button_vol1.png"));
        _label2->setPixmap(QPixmap(":/V/Resources/button_speed1.png"));
        _label3->setPixmap(QPixmap(":/V/Resources/nkplayer2.png"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_mini1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button5->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_full1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button6->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_close1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));

        //列表button样式
        buttonStyle2=QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:27px;color:rgb(100, 100, 100);}QPushButton{background-color:transparent;}QPushButton:hover{background-color:black}");
        //时间label样式
        labelStyle1=QStringLiteral(" font-family:'微软雅黑';font:bold;font-size:20px;color:black; background:transparent;");
        //音量、速度slider样式
        sliderStyle1=" QSlider::add-page:Horizontal{background-color:rgb(140, 150, 160); height:4px;} QSlider::sub-page:Horizontal{background-color:black; height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;margin: -8 0px;}";
        //主进度条slider样式
        sliderStyle2=" QSlider::add-page:Horizontal{background-color: rgb(140, 150, 160);height:4px;} QSlider::sub-page:Horizontal{background-color:black;height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;margin: -8 0px;}";
        break;

    case 7:
        //窗口初始背景颜色
        this->setStyleSheet("background-color: rgb(150,250,100)");
        if(isPause)_button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_play1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        else _button0->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_pause1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button2->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_videofile1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button3->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfile1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button8->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subdelay1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button7->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_subfront1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button1->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_suberror1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button9->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_screenshot1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button10->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_changestyle1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _label1->setPixmap(QPixmap(":/V/Resources/button_vol1.png"));
        _label2->setPixmap(QPixmap(":/V/Resources/button_speed1.png"));
        _label3->setPixmap(QPixmap(":/V/Resources/nkplayer2.png"));
        _button4->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_mini1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button5->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_full1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));
        _button6->setStyleSheet(QStringLiteral("QPushButton{background-color:transparent;background-image:url(:/V/Resources/button_close1.png);}QPushButton:hover{background-color:rgb(140, 150, 160)}"));

        //列表button样式
        buttonStyle2=QStringLiteral("QPushButton{font-family:'华文中宋';font:bold;font-size:27px;color:rgb(100, 100, 100);}QPushButton{background-color:transparent;}QPushButton:hover{background-color:black}");
        //时间label样式
        labelStyle1=QStringLiteral(" font-family:'微软雅黑';font:bold;font-size:20px;color:black; background:transparent;");
        //音量、速度slider样式
        sliderStyle1=" QSlider::add-page:Horizontal{background-color:rgb(140, 150, 160); height:4px;} QSlider::sub-page:Horizontal{background-color:black; height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;margin: -8 0px;}";
        //主进度条slider样式
        sliderStyle2=" QSlider::add-page:Horizontal{background-color: rgb(140, 150, 160);height:4px;} QSlider::sub-page:Horizontal{background-color:black;height:4px;} QSlider::groove:Horizontal{background:transparent;height:6px;} QSlider::handle:Horizontal{height: 20px;width:20px;background-color:black;margin: -8 0px;}";
        break;
    default:break;
    }

    _label4->setStyleSheet(labelStyle1);
    _label5->setStyleSheet(labelStyle1);
    _label6->setStyleSheet(labelStyle1);
    _slider1->setStyleSheet(sliderStyle2);    
    _slider2->setStyleSheet(sliderStyle1);
    _label3->setToolTip(QStringLiteral("###欢迎使用NKPlayer!\n————————————\n###操作指南:\n打开文件 F\n打开字幕 L\n字幕提前 <\n字幕滞后 >\n\
字幕乱码 K\n暂停\\播放 Space\\左键单击\n全屏\\活动 Enter\\鼠标中键\n退出 Esc\n音量+ UP\n\
音量- DOWN\n前进 RIGHT\n后退 LEFT\n视频定位 右键单击\n截屏 J\n重设NotePad++路径 G\n切换皮肤 H\n*鼠标左键点击视频可以暂停/播放\n*点击鼠标中键可以全屏/退出全屏\n*鼠标右键在水平方向不同位置点击可以迅速跳转进度\n*鼠标按住下方工具栏并拖动可以移动窗口\n*转动滚轮可以显示播放列表\n*点击播放列表可以播放相应视频\n*将视频文件拖入窗口可以直接播放\n————————————\n###注意:字幕乱码时需要在\n弹出的窗口中编码一栏选择\n转换为\
ANSI或其他编码，然\n后点击保存，最后关闭弹出\n程序即可\n————————————\n###有问题联系:\n1204219458@qq.com"));
    _slider3->setStyleSheet(sliderStyle1);
    _button11->setStyleSheet(buttonStyle2);
    _button12->setStyleSheet(buttonStyle2);
    _button13->setStyleSheet(buttonStyle2);
    _button14->setStyleSheet(buttonStyle2);
    _button15->setStyleSheet(buttonStyle2);
    _button16->setStyleSheet(buttonStyle2);
    _button17->setStyleSheet(buttonStyle2);

}

void VideoWidget::paintEvent(QPaintEvent *)
{
    //根据窗口的大小，设置列表button的长度
    _button11->setGeometry(0,(this->height()-200)*1/7,this->width(),40*this->height()/660);
    _button12->setGeometry(0,(this->height()-200)*2/7,this->width(),40*this->height()/660);
    _button13->setGeometry(0,(this->height()-200)*3/7,this->width(),40*this->height()/660);
    _button14->setGeometry(0,(this->height()-200)*4/7,this->width(),40*this->height()/660);
    _button15->setGeometry(0,(this->height()-200)*5/7,this->width(),40*this->height()/660);
    _button16->setGeometry(0,(this->height()-200)*6/7,this->width(),40*this->height()/660);
    _button17->setGeometry(0,(this->height()-200)*7/7,this->width(),40*this->height()/660);
}

VideoWidget::~VideoWidget()
{
    delete timerGetTime;
    delete timerDelayGetTime;
    delete nkplayer;
    delete this;
}
