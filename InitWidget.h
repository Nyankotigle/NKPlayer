#ifndef INITWIDGET_H
#define INITWIDGET_H


#include "MusicWidget.h"
#include "VideoWidget.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QLabel>

extern QString filePathArg;

class InitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InitWidget(QWidget *parent = nullptr);
    ~InitWidget();

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent*);

    QLabel* _label1;
    QLabel* _label2;
    QStringList vWordsList , mWordsList;

    QTimer* closeTimer;
    int status = 0;

    VideoWidget* videowidget;
    MusicWidget* musicwidget;

signals:

public slots:
    void chooseVideo();
    void chooseMusic();
    void slotClose();
};

#endif // INITWIDGET_H
