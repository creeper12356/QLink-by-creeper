#pragma once
#ifndef STATEBAR_H
#define STATEBAR_H
#include <QTimer>
#include <QPainter>
#include <QRectF>
#include <QDebug>
#include "config.h"

//游戏倒计时
class Clock:public QObject
{
    friend class StateBar;
    Q_OBJECT
private:
    QRectF drawBox;
private:
    int gameTime;//游戏总时间
    int curTime;//当前时间
    int updateInterval;//更新频率
    QTimer* tm;
private slots:
    void updateTime();
public:
    Clock(int gTime);
    ~Clock();

    int getCurTime() const;

    void start();
    void pause();
    void reset();
    void setTime(int time);
    void addTime(int time);
signals:
    void timeout();
public:
    const QRectF& getDrawBox() const{return drawBox;}
    void draw(QPainter& painter) const;
};
// 游戏状态栏
class StateBar:public QObject
{
    Q_OBJECT

private:
    QRectF drawBox;//更新范围

    Clock timer;
public:
    StateBar();
    ~StateBar();
    const QRectF& getDrawBox(){return drawBox;}
    void setDrawBox(const QRectF& b);
    void setPos(QPointF pt);
    void setPos(qreal x,qreal y);

    void draw(QPainter& painter);
private slots:
//    void timeOutSlot();
signals:
//    void clockTimeOut();
};

#endif // STATEBAR_H
