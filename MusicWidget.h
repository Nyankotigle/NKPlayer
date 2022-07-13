#ifndef MUSICWIDGET_H
#define MUSICWIDGET_H


#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>
#include <QCompleter>
#include <QLabel>
#include <QProcess>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QCursor>//光标类
#include <QIcon>//图标


class MusicWidget : public  QMainWindow
{
    Q_OBJECT
public:
    explicit MusicWidget(QWidget *parent = nullptr);
    ~MusicWidget();

    QString mplayerDir;//mplayer的地址
    QMenu* _menu;

    QProcess MusicProcess;
    QString _strMusicFolder;
    QStringList musicPathList;
    QStringList musicNameList;
    QCompleter* musicNameCptr;
    QLabel* _label1;
    QLabel* _label2;
    QLabel* _label3;
    QLabel* _label4;
    QLabel* _label5;
    QSlider* _slider1;
    QSlider* _slider2;
    QSlider* _slider3;
    QSlider* _slider4;
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
    int _button5Value;
    int _button6Value;
    int _button7Value;
    int _button8Value;
    int _button9Value;
    int _button10Value;
    int _button11Value;
    QLineEdit* _serchMusic;

    bool m_isPressed = false;//标识鼠标是否按住窗口
    bool isPause = true;//标识是否处于暂停状态
    QPoint m_startMovePos;//记录鼠标拖动窗口的起始位置
    int listShowMid;//显示歌曲列表中的中间一项在整个列表中的序号
    int nowPlay;//当前播放歌曲在整个列表中的序号
    QList<int> playRecord;//播放序号记录
    int backONrecord = 0;//记录返回到播放记录的从后往前第几首

    bool slider3_isPressed = false;//标识是否在拖动slider3
    bool key_RL_isPressed = false;//标识是否按下左右方向键
    bool canReadTimeValue = true;//是否可以读取查询到的视频时间信息
    int secAll, secNow;//视频的总秒数，当前秒数
    int nowStyleKind;//当前样式
    QTimer* timerGetTime;
    QTimer* timerDelayGetTime;
    QTimer* rePaintTimer;//重绘计时器

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void wheelEvent(QWheelEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);
    void SetStyle();
    void ShowList();

    bool eventFilter(QObject * object, QEvent * event);
signals:

public slots:
    void slotButton1Clicked();
    void slotButton2Clicked();
    void slotButton3Clicked();
    void slotButton4Clicked();
    void slotButton5_11Clicked();
    void slotOnSlider1Changed(int);
    void slotOnSlider3Released();//slider3滑块被释放
    void slotOnslider3Pressed();//slider3滑块被按下
    void slotOnslider3ValueChanged(int);//slider3值改变
    void slotOnslider4ValueChanged(int);//slider4值改变
    void slotGetTime();
    void slotTimerDelayGetTimeOut();
    void slotSerchMusic(const QString &);
    void slotExit();
    void slotMini();
    void slotChangeStyle();

};

#endif // MUSICWIDGET_H
