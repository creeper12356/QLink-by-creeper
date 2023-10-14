#pragma once
#ifndef CLOCK_H
#define CLOCK_H
#include "tools/indexbar.h"
/*
 * 游戏倒计时类
 */
class Clock: public IndexBar
{
    Q_OBJECT
private:
    int updateInterval;//update time interval
    QTimer* tm;
private slots:
    void updateTime();
public:
    Clock(QWidget* parent,int gameTime);
    ~Clock();

    //getters
    int getCurTime() const;
    int getGameTime() const;

    void setUpdateInterval(int interval);
    void start();
    void pause();
    void reset();
    void setTime(int time);
    void addTime(int time);
signals:
    void timeout();
};

#endif // CLOCK_H
