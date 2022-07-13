#ifndef VideoWidget_H
#define VideoWidget_H

#include <QWidget>
#include <QProcess>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QDateTime>
#include <QScreen>
#include <QGuiApplication>

extern QString filePathArg;

class VideoWidget : public QWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget *parent = nullptr);
    ~VideoWidget();

    QString mplayerDir;
    QProcess VideoProcess;
    QProcess SubtransProcess;
    QWidget _videoShowWidget;//视频输出窗口
    QSlider* _slider1;
    QSlider* _slider2;
    QSlider* _slider3;
    QPushButton* _button0;
    QPushButton* _button1;
    QPushButton* _button2;
    QPushButton* _button3;
    QPushButton* _button4;
    QPushButton* _button5;
    QPushButton* _button6;
    QPushButton* _button7;
    QPushButton* _button8;
    QPushButton* _button9;
    QPushButton* _button10;
    QPushButton* _button11;
    QPushButton* _button12;
    QPushButton* _button13;
    QPushButton* _button14;
    QPushButton* _button15;
    QPushButton* _button16;
    QPushButton* _button17;
    int _button11Value;
    int _button12Value;
    int _button13Value;
    int _button14Value;
    int _button15Value;
    int _button16Value;
    int _button17Value;
    QVBoxLayout* lay;
    QHBoxLayout* lay1;
    QHBoxLayout* lay2;
    QHBoxLayout* lay3;
    QLabel* _label1;
    QLabel* _label2;
    QLabel* _label3;
    QLabel* _label4;
    QLabel* _label5;
    QLabel* _label6;//当前播放
    QLabel* _label7;//标题栏Icon
    bool toolsHide = false;//标识工具栏是否处于隐藏状态
    bool isPause = false;//标识视频是否处于暂停状态
    bool m_isPressed;//标识鼠标是否按住窗口
    QPoint m_startMovePos;//记录鼠标拖动窗口的起始位置

    bool slider1_isPressed = false;//标识是否在拖动slider1
    bool key_RL_isPressed = false;//标识是否按下左右方向键
    bool canReadTimeValue = true;//是否可以读取查询到的视频时间信息
    int secAll, secNow;//视频的总秒数，当前秒数
    int nowStyleKind;
    QTimer* timerGetTime;
    QTimer* timerDelayGetTime;
    QTimer* timerDelayHideList;
    QPixmap* nkplayer = nullptr;

    QString _strDir;//保存初始打开文件目录
    QString _strSub;
    QString _strScreenshot;
    QString _strNotePad;
    QStringList videoPathList;//记录同文件夹下的视频列表
    QStringList videoNameList;
    int listShowMid = 0;//显示的文件列表的中间一项在整个列表中的位置
    int nowPlayInList;//当前播放视频在列表中的位置
    QString dropFilePath;//存储拖入文件的路径

    void hideORshowToolsbar();
    void Screenshot();
    void SetAllStyle(int);
    void ReadSetting();
    //bool event(QEvent*);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    //void resizeEvent(QResizeEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *);
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvnet(QDropEvent* event);

signals:
public slots:
    void slotOnSlider1Released();//slider1滑块被释放
    void slotOnslider1Pressed();//slider1滑块被按下
    void slotOnslider1ValueChanged(int);//slider1值改变
    void slotOnSlider2Changed(int);
    void slotOnSlider3Changed(int);
    void slotButton0Clicked();
    void slotButton1Clicked();
    void slotButton2Clicked();
    void slotButton3Clicked();
    void slotButton4Clicked();
    void slotButton5Clicked();
    void slotButton6Clicked();
    void slotButton7Clicked();
    void slotButton8Clicked();
    void slotButton9Clicked();
    void slotButton10Clicked();
    void slotButton11_17Clicked();
    void slotGetTime();
    void slotTimerDelayGetTimeOut();
    void slotTimerDelayHideListOut();
    void slotSubTransformed(int);
};

#endif // VideoWidget_H
